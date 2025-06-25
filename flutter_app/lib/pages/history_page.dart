import 'package:smart_weather_iot/utils/alert_helper.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';
import 'dart:math';

class HistoryPage extends StatefulWidget {
  const HistoryPage({super.key});

  @override
  State<HistoryPage> createState() => _HistoryPageState();
}

class _HistoryPageState extends State<HistoryPage> {
  final List<String> chartTypes = [
    'Temperature Indoor',
    'Temperature Outdoor',
    'Humidity Indoor',
    'Humidity Outdoor',
    'UV Index',
    'Wind Speed',
    'Pressure',
    'Altitude',
  ];
  String selectedChart = 'Temperature Indoor';

  List<Map<String, dynamic>> sensorData = [];

  @override
  void initState() {
    super.initState();
    fetchSensorData();
  }

  Future<void> fetchSensorData() async {
    final now = DateTime.now();
    final oneDayAgo = now.subtract(const Duration(hours: 24));

    final querySnapshot = await FirebaseFirestore.instance
        .collection('sensor_data')
        .orderBy('timestamp')
        .where('timestamp', isGreaterThanOrEqualTo: Timestamp.fromDate(oneDayAgo))
        .get();

    // Group by minute (keep one entry per minute)
    final grouped = <String, Map<String, dynamic>>{};
    for (var doc in querySnapshot.docs) {
      final data = doc.data();
      final ts = (data['timestamp'] as Timestamp).toDate();
      final minuteKey = DateFormat('yyyy-MM-dd HH:mm').format(ts);
      if (!grouped.containsKey(minuteKey)) {
        grouped[minuteKey] = {
          'temperature_indoor': (data['temperature_indoor'] ?? 0).toDouble(),
          'temperature_outdoor': (data['temperature_outdoor'] ?? 0).toDouble(),
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

    setState(() {
      sensorData = filteredGrouped
        ..sort((a, b) => (a['timestamp'] as DateTime).compareTo(b['timestamp']));
    });
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
                      color: color,
                      barWidth: 2,
                      dotData: FlDotData(show: true),
                    ),
                  ],
                  titlesData: FlTitlesData(
                    leftTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        reservedSize: 40,
                        interval: interval,
                      ),
                    ),
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        interval: (spots.length / 6).floorToDouble().clamp(1, double.infinity),
                        getTitlesWidget: (value, meta) {
                          int index = value.toInt();
                          if (index >= 0 && index < sensorData.length - 1) {
                            final timestamp = sensorData[index]['timestamp'] as DateTime;
                            return SideTitleWidget(
                              axisSide: meta.axisSide,
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
            const SizedBox(height: 10),
            Text(
              "🔺 Max: ${maxY.toStringAsFixed(1)}   🔻 Min: ${minY.toStringAsFixed(1)}",
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
        case 'Temperature Indoor':
          y = data['temperature_indoor'];
          break;
        case 'Temperature Outdoor':
          y = data['temperature_outdoor'];
          break;
        case 'Humidity Indoor':
          y = data['humidity_indoor'];
          break;
        case 'Humidity Outdoor':
          y = data['humidity_outdoor'];
          break;
        case 'UV Index':
          y = data['uv_index'];
          break;
        case 'Wind Speed':
          y = data['wind_speed'];
          break;
        case 'Pressure':
          y = data['pressure'];
          break;
        case 'Altitude':
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
      appBar: AppBar(title: const Text('Sensor History')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
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
            Expanded(
              child: buildChart(
                getChartData(selectedChart),
                selectedChart,
                Colors.blue,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
