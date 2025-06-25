
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class AlertMessage {
  final String title;
  final String content;
  AlertMessage(this.title, this.content);
}

class AlertHelper {
  static DateTime _lastTempAlertTime = DateTime.now().subtract(Duration(minutes: 1));
  static DateTime _lastHumidityAlertTime = DateTime.now().subtract(Duration(minutes: 1));
  static DateTime _lastWindAlertTime = DateTime.now().subtract(Duration(minutes: 1));
  static DateTime _lastUvAlertTime = DateTime.now().subtract(Duration(minutes: 1));
  static DateTime _lastPlantWaterAlertTime = DateTime.now().subtract(Duration(minutes: 1));

  static BuildContext? _globalContext;
  static final List<AlertMessage> _alertQueue = [];
  static bool _isAlertShowing = false;

  static void setContext(BuildContext context) {
    _globalContext = context;
  }

  static Future<void> checkAlerts(Map<String, dynamic> data) async {
    final context = _globalContext;
    if (context == null || !context.mounted) return;

    final prefs = await SharedPreferences.getInstance();
    final now = DateTime.now();

    final bool tempAlert = prefs.getBool('tempAlert') ?? true;
    final bool humidityAlert = prefs.getBool('humidityAlert') ?? true;
    final bool windAlert = prefs.getBool('windAlert') ?? true;
    final bool uvAlert = prefs.getBool('uvAlert') ?? true;
    final bool plantWaterAlert = prefs.getBool('plantWaterAlert') ?? true;

    final double temp = double.tryParse(data['temperature_outdoor']?.toString() ?? '') ?? 0;
    final double tempThreshold = prefs.getDouble('tempThreshold') ?? 30.0;
    final double hum = double.tryParse(data['humidity_outdoor']?.toString() ?? '') ?? 0;
    final double humidityThreshold = prefs.getDouble('humidityThreshold') ?? 70.0;
    final double wind = double.tryParse(data['wind_speed']?.toString() ?? '') ?? 0;
    final double windThreshold = prefs.getDouble('windThreshold') ?? 20.0;
    final double uv = double.tryParse(data['uv_index']?.toString() ?? '') ?? 0;
    final double uvThreshold = prefs.getDouble('uvThreshold') ?? 6.0;
    if (tempAlert && temp > tempThreshold && now.difference(_lastTempAlertTime).inSeconds > 60) {
      _lastTempAlertTime = now;
      _enqueueAlert(AlertMessage("High Temperature", "It's hot outside! 🌡️"));
    }

    if (humidityAlert && hum > humidityThreshold && now.difference(_lastHumidityAlertTime).inSeconds > 60) {
      _lastHumidityAlertTime = now;
      _enqueueAlert(AlertMessage("High Humidity", "Humidity is very high! 💧"));
    }

    if (windAlert && wind > windThreshold && now.difference(_lastWindAlertTime).inSeconds > 60) {
      _lastWindAlertTime = now;
      _enqueueAlert(AlertMessage("Strong Wind", "It's very windy outside! 💨"));
    }

    if (uvAlert && uv > uvThreshold && now.difference(_lastUvAlertTime).inSeconds > 60) {
      _lastUvAlertTime = now;
      _enqueueAlert(AlertMessage("High UV Index", "Apply sunscreen! ☀️"));
    }
    if (plantWaterAlert && hum < 50 && temp < 30 && wind < 5 && uv < 3 && now.difference(_lastPlantWaterAlertTime).inSeconds > 60) {
      _lastPlantWaterAlertTime = now;
      _enqueueAlert(AlertMessage("Water the Plants", "Now is a great time to water the plants! 🌿💧"));
    }


    _processAlertQueue(context);
  }

  static void _enqueueAlert(AlertMessage message) {
    _alertQueue.add(message);
  }

  static void _processAlertQueue(BuildContext context) async {
    if (_isAlertShowing || _alertQueue.isEmpty) return;
    _isAlertShowing = true;

    while (_alertQueue.isNotEmpty) {
      final message = _alertQueue.removeAt(0);
      // ✅ Auto-dismiss dialog after 10 seconds OR when user presses OK
      await Future.any([
        showDialog(
          context: context,
          barrierDismissible: false,
          builder: (BuildContext context) => AlertDialog(
            title: Text(message.title),
            content: Text(message.content),
            actions: [
              TextButton(
                onPressed: () => Navigator.of(context).pop(),
                child: const Text("OK"),
              ),
            ],
          ),
        ),
        Future.delayed(const Duration(seconds: 10), () {
          if (Navigator.of(context).canPop()) {
            Navigator.of(context).pop();
          }
        }),
      ]);
    }

    _isAlertShowing = false;
  }
}
