import 'dart:convert';
import 'package:http/http.dart' as http;

class ApiService {
  static const String baseUrl = "http://<ESP32-IP-ADDRESS>/data"; // Replace with real IP

  static Future<Map<String, dynamic>> fetchSensorData() async {
    final response = await http.get(Uri.parse(baseUrl));

    if (response.statusCode == 200) {
      return jsonDecode(response.body);
    } else {
      throw Exception("Failed to load sensor data");
    }
  }
}
