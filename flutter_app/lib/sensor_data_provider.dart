
import 'dart:async';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:smart_weather_iot/utils/alert_helper.dart';

class SensorDataProvider {
  static final SensorDataProvider _instance = SensorDataProvider._internal();
  factory SensorDataProvider() => _instance;
  SensorDataProvider._internal() {
    _startListening();
  }

  final StreamController<Map<String, dynamic>> _controller =
  StreamController<Map<String, dynamic>>.broadcast();

  Stream<Map<String, dynamic>> get stream => _controller.stream;


  DateTime? _lastReceivedTime;
  bool _disconnected = false;
  Timer? _disconnectTimer;
  final StreamController<bool> _disconnectController = StreamController<bool>.broadcast();

  Stream<bool> get disconnectStream => _disconnectController.stream;

  void _startListening() {

    FirebaseFirestore.instance
        .collection('sensor_data')
        .orderBy('timestamp', descending: true)
        .limit(1)
        .snapshots()
        .listen((snapshot) {
      if (snapshot.docs.isNotEmpty) {
        final data = snapshot.docs.first.data();
        AlertHelper.checkAlerts(data);

        _lastReceivedTime = DateTime.now();
        _controller.add(data);

        if (_disconnected) {
          _disconnected = false;
          _disconnectController.add(false);
        }

        _disconnectTimer?.cancel();
        _disconnectTimer = Timer(Duration(seconds: 30), () {
          _disconnected = true;
          _disconnectController.add(true);
        });

      }
    });
  }
}
