import 'package:flutter/material.dart';

class ForecastPage extends StatelessWidget {
  const ForecastPage({super.key});

  @override
  Widget build(BuildContext context) {
    final forecast = [
      {'day': 'Monday', 'temp': 25, 'condition': 'Sunny'},
      {'day': 'Tuesday', 'temp': 23, 'condition': 'Cloudy'},
      {'day': 'Wednesday', 'temp': 22, 'condition': 'Rain'},
    ];

    return Scaffold(
      appBar: AppBar(title: const Text("Weather Forecast")),
      body: ListView.builder(
        itemCount: forecast.length,
        itemBuilder: (context, index) {
          final day = forecast[index];
          return Card(
            child: ListTile(
              leading: Icon(
                day['condition'] == 'Sunny'
                    ? Icons.wb_sunny
                    : day['condition'] == 'Rain'
                    ? Icons.beach_access
                    : Icons.cloud,
              ),
              title: Text(day['day']),  // Cast to String
              subtitle: Text("${day['temp']}°C - ${day['condition']}"),
            ),
          );
        },
      ),
    );
  }
}
