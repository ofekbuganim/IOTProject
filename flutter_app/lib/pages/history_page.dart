import 'package:flutter/material.dart';

class HistoryPage extends StatelessWidget {
  const HistoryPage({super.key});

  @override
  Widget build(BuildContext context) {
    final history = [
      {'time': '08:00', 'temp': 22.5},
      {'time': '09:00', 'temp': 23.1},
      {'time': '10:00', 'temp': 24.0},
    ];

    return Scaffold(
      appBar: AppBar(title: const Text("Weather History")),
      body: ListView.builder(
        itemCount: history.length,
        itemBuilder: (context, index) {
          final entry = history[index];
          return ListTile(
            leading: const Icon(Icons.access_time),
            title: Text("Time: ${entry['time']}"),
            subtitle: Text("Temperature: ${entry['temp']}°C"),
          );
        },
      ),
    );
  }
}
