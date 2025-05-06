import 'package:flutter/material.dart';
import '../services/api_service.dart';

class RealTimePage extends StatefulWidget {
  const RealTimePage({super.key});

  @override
  State<RealTimePage> createState() => _RealTimePageState();
}

class _RealTimePageState extends State<RealTimePage> {
  Map<String, dynamic>? data;
  bool isLoading = true;
  String error = "";

  Future<void> loadData() async {
    setState(() {
      isLoading = true;
      error = "";
    });

    try {
      final result = await ApiService.fetchSensorData();
      setState(() {
        data = result;
        isLoading = false;
      });
    } catch (e) {
      setState(() {
        error = e.toString();
        isLoading = false;
      });
    }
  }

  @override
  void initState() {
    super.initState();
    loadData();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Real-Time Weather"),
        actions: [
          IconButton(
            icon: const Icon(Icons.refresh),
            onPressed: loadData,
          )
        ],
      ),
      body: isLoading
          ? const Center(child: CircularProgressIndicator())
          : error.isNotEmpty
          ? Center(child: Text("Error: $error"))
          : Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            const Text(
              "Main Station Data",
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            Text("Temperature: ${data!['main']['temperature']}°C"),
            Text("Humidity: ${data!['main']['humidity']}%"),
            Text("Wind Speed: ${data!['main']['wind']} m/s"),
            const SizedBox(height: 20),
            const Text(
              "Secondary Station Data",
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            Text("Temperature: ${data!['secondary']['temperature']}°C"),
            Text("Humidity: ${data!['secondary']['humidity']}%"),
            Text("Wind Speed: ${data!['secondary']['wind']} m/s"),
          ],
        ),
      ),
    );
  }
}
