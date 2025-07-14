import 'package:smart_weather_iot/utils/alert_helper.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';
import 'dart:math';
import 'package:connectivity_plus/connectivity_plus.dart';

class HistoryPage extends StatefulWidget {
  const HistoryPage({super.key});

  @override
  State<HistoryPage> createState() => _HistoryPageState();
}

bool isFetching = false;
List<Map<String, dynamic>> lastFetchedSensorData = [];

class _HistoryPageState extends State<HistoryPage> {
  String getUvRiskLevel(double uv) {
    if (uv <= 2) return "Low";
    if (uv <= 5) return "Moderate";
    if (uv <= 7) return "High";
    if (uv <= 10) return "Very High";
    return "Extreme";
  }

  final List<String> chartTypes = [
    'Temperature Indoor (°C)',
    'Temperature Outdoor (°C)',
    'Humidity Indoor (%)',
    'Humidity Outdoor (%)',
    'UV Index (UV)',
    'Wind Speed (km/s)',
    'Pressure (atm)',
    'Altitude (m)',
  ];

  final Map<String, String> yAxisUnits = {
    'Temperature Indoor': '°C',
    'Temperature Outdoor': '°C',
    'Humidity Indoor': '%',
    'Humidity Outdoor': '%',
    'UV Index': 'UV',
    'Wind Speed': 'km/s',
    'Pressure': 'atm',
    'Altitude': 'm',
  };

  String selectedChart = 'Temperature Indoor (°C)';

  List<Map<String, dynamic>> sensorData = [];
  bool isLoading = true;
  bool hasInternet = true;
  bool isStationConnected = true; // later connect it to your real disconnection flag
  bool isCheckingConnectivity = true;
  //bool isFetching = false;

  @override
  void initState() {
    super.initState();

    // 🔌 Check initial connectivity once on startup
    Connectivity().checkConnectivity().then((result) {
      final hasConnection = result != ConnectivityResult.none;
      setState(() {
        hasInternet = hasConnection;
        isCheckingConnectivity = false;
      });
    });

    // 🔄 Start fetch and listen for changes
    fetchSensorData();

    Connectivity().onConnectivityChanged.listen((result) {
      final hasConnection = result != ConnectivityResult.none;
      setState(() {
        hasInternet = hasConnection;
      });
      if (hasConnection) {
        fetchSensorData();
      }
    });
  }

  Future<void> fetchSensorData() async {
    if (isFetching) {
      setState(() {
        sensorData = lastFetchedSensorData;
        isLoading = false;
      });
      return;
    }
    isFetching = true;
    setState(() {
      isLoading = true; // <-- ADD THIS
    });
    try {
      final now = DateTime.now();
      final oneHourAgo = now.subtract(const Duration(hours: 1));

      final querySnapshot = await FirebaseFirestore.instance
          .collection('sensor_data')
          .orderBy('timestamp')
          .where(
          'timestamp', isGreaterThanOrEqualTo: Timestamp.fromDate(oneHourAgo))
          .get();

      // Group by minute (keep one entry per minute)
      final grouped = <String, Map<String, dynamic>>{};
      for (var doc in querySnapshot.docs) {
        final data = doc.data();
        final ts = (data['timestamp'] as Timestamp).toDate();
        final minuteKey = DateFormat('yyyy-MM-dd HH').format(ts) + ':' +
            (ts.minute ~/ 10).toString().padLeft(2, '0');
        if (!grouped.containsKey(minuteKey)) {
          grouped[minuteKey] = {
            'temperature_indoor': (data['temperature_indoor'] ?? 0).toDouble(),
            'temperature_outdoor': (data['temperature_outdoor'] ?? 0)
                .toDouble(),
            'humidity_indoor': (data['humidity_indoor'] ?? 0).toDouble(),
            'humidity_outdoor': (data['humidity_outdoor'] ?? 0).toDouble(),
            'uv_index': (data['uv_index'] ?? 0).toDouble(),
            'wind_speed': (data['wind_speed'] ?? 0).toDouble(),
            'pressure': (data['pressure'] ?? 0).toDouble(),
            'altitude': (data['altitude'] ?? 0).toDouble(),
            'timestamp': ts,
          };
        }
      }


      // Filter grouped sensor data to only include entries from the last hour
      final oneHourAgoLocal = DateTime.now().subtract(const Duration(hours: 1));
      final filteredGrouped = grouped.values.where((entry) {
        final ts = entry['timestamp'] as DateTime;
        return ts.isAfter(oneHourAgoLocal);
      }).toList();

      if (!mounted) return;

      lastFetchedSensorData = filteredGrouped;

      setState(() {
        sensorData = lastFetchedSensorData
          ..sort((a, b) =>
              (a['timestamp'] as DateTime).compareTo(b['timestamp']));
        isLoading = false;
      });
    } catch (e) {
      print('❌ Error fetching sensor data: $e');
      if (!mounted) return;
      setState(() {
        sensorData = []; // Set empty so chart shows fallback text
        isLoading = false;
      });
    }
    finally {
      isFetching = false; // ✅ allow next fetch
    }
  }

  // (Same imports and class declarations as before...)

  Widget buildChart(List<FlSpot> spots, String title, Color color) {
    if (spots.isEmpty) {
      return const Text("No data to display");
    }

    final minY = spots.map((e) => e.y).reduce(min);
    final maxY = spots.map((e) => e.y).reduce(max);

    double getCleanInterval(double min, double max) {
      final diff = max - min;
      if (diff <= 0) return 1;
      final raw = diff / 4;
      final pow10 = pow(10, (log(raw) / ln10).floor());
      final norm = raw / pow10;
      double rounded;
      if (norm < 1.5) {
        rounded = 1;
      } else if (norm < 3) {
        rounded = 2;
      } else if (norm < 7) {
        rounded = 5;
      } else {
        rounded = 10;
      }
      return rounded * pow10;
    }

    double interval = getCleanInterval(minY, maxY);
    final adjustedMinY = (minY / interval).floor() * interval;
    final adjustedMaxY = (maxY / interval).ceil() * interval;

    return Card(
      margin: const EdgeInsets.symmetric(vertical: 10),
      child: Padding(
        padding: const EdgeInsets.all(12),
        child: Column(
          children: [
            Text(title, style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
            SizedBox(
              height: 250,
              child: LineChart(
                LineChartData(
                  minY: adjustedMinY,
                  maxY: adjustedMaxY,
                  gridData: FlGridData(show: true),
                  borderData: FlBorderData(show: true),
                  lineBarsData: [
                    LineChartBarData(
                      spots: spots,
                      isCurved: true,
                      color: Colors.white,
                      barWidth: 2,
                      dotData: FlDotData(
                        show: true,
                        getDotPainter: (spot, percent, barData, index) => FlDotCirclePainter(
                          radius: 4,
                          color: Colors.white,
                          strokeWidth: 1.5,
                          strokeColor: Colors.blue, // optional: outline
                        ),
                      ),
                    ),
                  ],
                  titlesData: FlTitlesData(
                    leftTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        reservedSize: 40,
                        interval: interval,
                        getTitlesWidget: (value, meta) {
                          return SideTitleWidget(
                            axisSide: meta.axisSide,
                            child: Text(
                              value.toStringAsFixed(1), // 👈 Always show 1 decimal (e.g. 25.8)
                              style: const TextStyle(fontSize: 11),
                            ),
                          );
                        },
                      ),
                    ),
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        interval: max(1, (spots.length / 5).floorToDouble()),
                        getTitlesWidget: (value, meta) {
                          int index = value.toInt();
                          if (index >= 0 && index < sensorData.length - 1) {
                            final timestamp = sensorData[index]['timestamp'] as DateTime;
                            return SideTitleWidget(
                              axisSide: meta.axisSide,
                              space: 8,
                              child: Text(
                                DateFormat.Hm().format(timestamp),
                                style: const TextStyle(fontSize: 10),
                              ),
                            );
                          }
                          return const SizedBox.shrink();
                        },
                      ),
                    ),
                    rightTitles: AxisTitles(sideTitles: SideTitles(showTitles: false)),
                    topTitles: AxisTitles(sideTitles: SideTitles(showTitles: false)),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 8),
            const Text(
              'Time',
              style: TextStyle(fontSize: 12, color: Colors.grey),
            ),
            const SizedBox(height: 10),
            Text(
              "🔺 Max: ${(maxY == 0.0 ? '0.0' : (maxY < 10 ? maxY.toStringAsFixed(2) : maxY.toStringAsFixed(1)))}  🔻 Min: ${(minY == 0.0 ? '0.0' : (minY < 10 ? minY.toStringAsFixed(2) : minY.toStringAsFixed(1)))}",
              style: const TextStyle(fontSize: 14, color: Colors.grey),
            ),
          ],
        ),
      ),
    );
  }


  List<FlSpot> getChartData(String chartType) {
    return List<FlSpot>.generate(sensorData.length, (index) {
      final data = sensorData[index];
      double y = 0;
      switch (chartType) {
        case 'Temperature Indoor (°C)':
          y = data['temperature_indoor'];
          break;
        case 'Temperature Outdoor (°C)':
          y = data['temperature_outdoor'];
          break;
        case 'Humidity Indoor (%)':
          y = data['humidity_indoor'];
          break;
        case 'Humidity Outdoor (%)':
          y = data['humidity_outdoor'];
          break;
        case 'UV Index (UV)':
        case 'UV Index (UV)':
          y = (data['uv_index'] ?? 0).toDouble();
          if (y.isNaN || y.isInfinite || y < 0) y = 0;
          break;
        case 'Wind Speed (km/s)':
          y = data['wind_speed'];
          break;
        case 'Pressure (atm)':
          y = data['pressure'];
          break;
        case 'Altitude (m)':
          y = data['altitude'];
          break;
      }
      return FlSpot(index.toDouble(), y);
    });
  }

  @override
  Widget build(BuildContext context) {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      AlertHelper.setContext(context);
    });
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sensor History'),
        actions: (!isCheckingConnectivity && hasInternet && isStationConnected)
            ? [
          IconButton(
            icon: const Icon(Icons.refresh),
            tooltip: 'Refresh',
            onPressed: () {
              fetchSensorData();
            },
          ),
        ]
            : [],
      ),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: SingleChildScrollView(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              DropdownButton<String>(
                value: selectedChart,
                items: chartTypes.map((type) {
                  return DropdownMenuItem<String>(
                    value: type,
                    child: Text(type),
                  );
                }).toList(),
                onChanged: (value) {
                  setState(() {
                    selectedChart = value!;
                  });
                },
              ),
              const SizedBox(height: 16),
              isLoading
                  ? const Center(child: CircularProgressIndicator()) :
              buildChart(
                getChartData(selectedChart),
                selectedChart,
                Colors.blue,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
