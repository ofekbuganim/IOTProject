import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:firebase_messaging/firebase_messaging.dart';
import 'forecast_page.dart';
import 'package:smart_weather_iot/utils/alert_helper.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  bool tempAlertEnabled = false;
  bool humidityAlertEnabled = false;
  bool windAlertEnabled = false;
  bool uvAlertEnabled = false;

  double tempThreshold = 30.0;
  double humidityThreshold = 80.0;
  double windThreshold = 15.0;
  double uvThreshold = 7.0;

  Map<String, DateTime> lastAlertTime = {};

  bool hasSeenFirstSensorUpdate = false;

  @override
  void initState() {
    super.initState();
    _loadAlertPrefs();
    _initFirebaseMessaging(); // <--- New
  }

  Future<void> _initFirebaseMessaging() async {
    FirebaseMessaging messaging = FirebaseMessaging.instance;

    // Optional: Request permission (for iOS)
    await messaging.requestPermission();

    // Print the device token (you’ll use it from backend to send push messages)
    String? token = await messaging.getToken();
    print("📱 FCM Token: $token");

    // Foreground notifications
    FirebaseMessaging.onMessage.listen((RemoteMessage message) {
      if (message.notification != null && mounted) {
        showDialog(
          context: context,
          builder: (_) => AlertDialog(
            title: Text(message.notification!.title ?? "Notification"),
            content: Text(message.notification!.body ?? ""),
            actions: [
              TextButton(
                onPressed: () => Navigator.pop(context),
                child: const Text("OK"),
              )
            ],
          ),
        );
      }
    });

    // Background/terminated notification clicked
    FirebaseMessaging.onMessageOpenedApp.listen((RemoteMessage message) {
      debugPrint("📬 Notification opened: ${message.notification?.title}");
    });

    // App launched by notification
    RemoteMessage? initialMessage = await messaging.getInitialMessage();
    if (initialMessage != null && initialMessage.notification != null) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        showDialog(
          context: context,
          builder: (_) => AlertDialog(
            title: Text(initialMessage.notification!.title ?? "Notification"),
            content: Text(initialMessage.notification!.body ?? ""),
            actions: [
              TextButton(
                onPressed: () => Navigator.pop(context),
                child: const Text("OK"),
              )
            ],
          ),
        );
      });
    }
  }

  Future<void> _loadAlertPrefs() async {
    final prefs = await SharedPreferences.getInstance();
    setState(() {
      tempAlertEnabled = prefs.getBool('tempAlert') ?? false;
      humidityAlertEnabled = prefs.getBool('humidityAlert') ?? false;
      windAlertEnabled = prefs.getBool('windAlert') ?? false;
      uvAlertEnabled = prefs.getBool('uvAlert') ?? false;

      tempThreshold = prefs.getDouble('tempThreshold') ?? 30.0;
      humidityThreshold = prefs.getDouble('humidityThreshold') ?? 80.0;
      windThreshold = prefs.getDouble('windThreshold') ?? 15.0;
      uvThreshold = prefs.getDouble('uvThreshold') ?? 7.0;
    });
  }

  bool _canShowAlert(String key) {
    final now = DateTime.now();
    if (!lastAlertTime.containsKey(key)) return true;
    return now.difference(lastAlertTime[key]!) > const Duration(minutes: 1);
  }

  void _checkAlerts(Map<String, dynamic> data) {
    final List<Map<String, String>> alertsToShow = [];

    final double temp = (data['temperature_outdoor'] as num).toDouble();
    final double hum = (data['humidity_outdoor'] as num).toDouble();
    final double wind = (data['wind_speed'] as num).toDouble();
    final double uv = (data['uv_index'] as num).toDouble();

    if (tempAlertEnabled && temp > tempThreshold && _canShowAlert('temp')) {
      alertsToShow.add({
        'key': 'temp',
        'title': "☀️ Sunscreen Reminder",
        'message': "Outdoor temperature is above $tempThreshold°C — apply sunscreen!"
      });
    }

    if (humidityAlertEnabled && hum > humidityThreshold && _canShowAlert('humidity')) {
      alertsToShow.add({
        'key': 'humidity',
        'title': "☔ Umbrella Alert",
        'message': "Outdoor humidity is above $humidityThreshold% — take an umbrella!"
      });
    }

    if (windAlertEnabled && wind > windThreshold && _canShowAlert('wind')) {
      alertsToShow.add({
        'key': 'wind',
        'title': "💨 Wind Alert",
        'message': "Wind speed is above $windThreshold km/s — be cautious outdoors!"
      });
    }

    if (uvAlertEnabled && uv > uvThreshold && _canShowAlert('uv')) {
      alertsToShow.add({
        'key': 'uv',
        'title': "☀️ UV Alert",
        'message': "UV index is above $uvThreshold — consider sun protection!"
      });
    }

    if (alertsToShow.isNotEmpty) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        _showMultipleAlerts(alertsToShow, 0);
      });
    }
  }

  void _showMultipleAlerts(List<Map<String, String>> alerts, int index) {
    if (index >= alerts.length || !mounted) return;

    final alert = alerts[index];
    final String key = alert['key']!;

    lastAlertTime[key] = DateTime.now();

    showDialog(
      context: context,
      builder: (_) => AlertDialog(
        title: Text(alert['title']!),
        content: Text(alert['message']!),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
              _showMultipleAlerts(alerts, index + 1);
            },
            child: const Text("OK"),
          )
        ],
      ),
    );
  }

  String getUvRiskLevel(double uv) {
    if (uv <= 2) return "Low";
    if (uv <= 5) return "Moderate";
    if (uv <= 7) return "High";
    if (uv <= 10) return "Very High";
    return "Extreme";
  }

  @override
  Widget build(BuildContext context) {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      AlertHelper.setContext(context);
    });
    final sensors = FirebaseFirestore.instance
        .collection('sensor_data')
        .orderBy('timestamp', descending: true)
        .limit(60)
        .snapshots();

    return Scaffold(
      appBar: AppBar(
        title: const Text('Live Sensor Data'),
        actions: [], // No actions in the AppBar
      ),
      body: StreamBuilder<QuerySnapshot>(
        stream: sensors,
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return const Center(child: CircularProgressIndicator());
          }

          if (!snapshot.hasData || snapshot.data!.docs.isEmpty) {
            return const Center(child: Text("No data found"));
          }

          final docs = snapshot.data!.docs;
          final latest = docs.first.data() as Map<String, dynamic>;

          

          Widget buildLine(String label, double value, String unit, String emoji) {
            return Text(
              "$emoji $label: ${value.toStringAsFixed(1)}$unit",
              style: const TextStyle(fontSize: 20),
            );
          }

          final double uvValue = (latest['uv_index'] as num).toDouble();

          return Padding(
            padding: const EdgeInsets.all(16),
          child: SingleChildScrollView(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                buildLine("Temperature Indoor", latest['temperature_indoor'], "°C", "🏠🌡"),
                const SizedBox(height: 16),
                buildLine("Temperature Outdoor", latest['temperature_outdoor'], "°C", "🌤"),
                const SizedBox(height: 16),
                buildLine("Humidity Indoor", latest['humidity_indoor'], "%", "🏠💧"),
                const SizedBox(height: 16),
                buildLine("Humidity Outdoor", latest['humidity_outdoor'], "%", "🌦"),
                const SizedBox(height: 16),
                buildLine("Wind Speed", latest['wind_speed'], " km/s", "💨"),
                const SizedBox(height: 16),
                Text(
                  "☀️ UV level: ${getUvRiskLevel(uvValue)}",
                  style: const TextStyle(fontSize: 20),
                ),
                const SizedBox(height: 16),
                buildLine("Pressure", latest['pressure'], " atm", "📈"),
                const SizedBox(height: 16),
                buildLine("Altitude", latest['altitude'], " m", "🏔"),
              ],
            ),
          ),
          );
        },
      ),
    );
  }
}
