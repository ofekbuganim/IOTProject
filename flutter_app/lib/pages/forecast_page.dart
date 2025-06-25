import 'package:smart_weather_iot/utils/alert_helper.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:math';
import 'package:fl_chart/fl_chart.dart';
import 'package:intl/intl.dart';

class ForecastPage extends StatefulWidget {
  const ForecastPage({Key? key}) : super(key: key);

  @override
  State<ForecastPage> createState() => _ForecastPageState();
}

class _ForecastPageState extends State<ForecastPage> {
  final String apiKey = '98aaff816946c128eef6f41d58d2b803';
  final double latitude = 32.7940;
  final double longitude = 34.9896;

  List<double> temperatures = [];
  List<double> humidities = [];
  List<double> windSpeeds = [];
  List<double> pressures = [];
  List<String> timestamps = [];

  bool isLoading = true;
  int hourLimit = 8;
  List<int> availableOptions = [8, 16, 24, 40];

  @override
  void initState() {
    super.initState();
    fetchForecastData();
  }

  Future<void> fetchForecastData() async {
    final url =
        'https://api.openweathermap.org/data/2.5/forecast?lat=$latitude&lon=$longitude&units=metric&appid=$apiKey';

    try {
      final response = await http.get(Uri.parse(url));
      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        final List forecastList = data['list'];

        setState(() {
          temperatures = forecastList.map<double>((item) => item['main']['temp']?.toDouble() ?? 0.0).toList();
          humidities = forecastList.map<double>((item) => item['main']['humidity']?.toDouble() ?? 0.0).toList();
          windSpeeds = forecastList.map<double>((item) => item['wind']['speed']?.toDouble() ?? 0.0).toList();
          pressures = forecastList.map<double>((item) => item['main']['pressure']?.toDouble() ?? 0.0).toList();
          timestamps = forecastList.map<String>((item) {
            final dt = DateTime.parse(item['dt_txt']);
            return DateFormat.Hm().format(dt);
          }).toList();

          isLoading = false;
        });
      } else {
        throw Exception('Failed to load forecast data');
      }
    } catch (e) {
      print('Error: $e');
      setState(() {
        isLoading = false;
      });
    }
  }

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

  double getXLabelInterval() {
    if (hourLimit == 8) return 1;
    if (hourLimit == 16) return 2;
    if (hourLimit == 24) return 3;
    if (hourLimit == 40) return 5;
    return 1;
  }

  Widget buildChart(String title, List<double> data, Color color) {
    final limitedData = data.take(hourLimit).toList();
    final limitedLabels = timestamps.take(hourLimit).toList();

    double minY = limitedData.reduce(min);
    double maxY = limitedData.reduce(max);
    double interval = getCleanInterval(minY, maxY);

    minY = (minY / interval).floor() * interval;
    maxY = (maxY / interval).ceil() * interval;

    return Card(
      margin: const EdgeInsets.symmetric(vertical: 10),
      child: Padding(
        padding: const EdgeInsets.all(8),
        child: Column(
          children: [
            Text(title, style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
            SizedBox(
              height: 200,
              child: LineChart(
                LineChartData(
                  minY: minY,
                  maxY: maxY,
                  gridData: FlGridData(show: true),
                  borderData: FlBorderData(show: true),
                  lineBarsData: [
                    LineChartBarData(
                      spots: limitedData.asMap().entries.map((e) => FlSpot(e.key.toDouble(), e.value)).toList(),
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
                        reservedSize: title == "Pressure" ? 48 : 40,
                        interval: interval,
                        getTitlesWidget: (value, meta) {
                          if (title == "Pressure") {
                            return Text(
                              '${(value / 1000).toStringAsFixed(3)}k',
                              style: TextStyle(fontSize: 10),
                            );
                          } else {
                            // Keep adjustable decimal formatting like before
                            final rounded = value.roundToDouble();
                            final display = (value == rounded)
                                ? rounded.toInt().toString()
                                : value.toStringAsFixed(1);
                            return Text(display, style: TextStyle(fontSize: 12));
                          }
                        },
                      ),
                    ),

                    rightTitles: AxisTitles(sideTitles: SideTitles(showTitles: false)),
                    topTitles: AxisTitles(sideTitles: SideTitles(showTitles: false)),
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        interval: getXLabelInterval(),
                        getTitlesWidget: (value, meta) {
                          int index = value.toInt();
                          if (index >= 0 && index < limitedLabels.length - 1) {
                            return SideTitleWidget(
                              axisSide: meta.axisSide,
                              child: Text(limitedLabels[index], style: const TextStyle(fontSize: 10)),
                            );
                          }
                          return const SizedBox.shrink(); // hide last to avoid overlap
                        },
                      ),
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      AlertHelper.setContext(context);
    });
    return Scaffold(
      appBar: AppBar(title: const Text('Weather Forecast')),
      body: isLoading
          ? const Center(child: CircularProgressIndicator())
          : SafeArea(
        child: SingleChildScrollView(
          padding: const EdgeInsets.all(16),
          child: Column(
            children: [
              DropdownButton<int>(
                value: hourLimit,
                onChanged: (newValue) {
                  setState(() {
                    hourLimit = newValue!;
                  });
                },
                items: availableOptions.map((int value) {
                  return DropdownMenuItem<int>(
                    value: value,
                    child: Text('Next ${value * 3} Hours'),
                  );
                }).toList(),
              ),
              buildChart('Temperature (°C)', temperatures, Colors.red),
              buildChart('Humidity (%)', humidities, Colors.blue),
              buildChart('Wind Speed (m/s)', windSpeeds, Colors.green),
              buildChart('Pressure (hPa)', pressures, Colors.purple),
            ],
          ),
        ),
      ),
    );
  }
}
