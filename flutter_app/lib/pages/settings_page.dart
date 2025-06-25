import 'package:smart_weather_iot/utils/alert_helper.dart';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class SettingsPage extends StatefulWidget {
  const SettingsPage({super.key});

  @override
  State<SettingsPage> createState() => _SettingsPageState();
}

class _SettingsPageState extends State<SettingsPage> {
  bool tempAlert = false;
  bool humidityAlert = false;
  bool windAlert = false;
  bool uvAlert = false;
  bool plantWaterAlert = false;

  double tempThreshold = 30.0;
  double humidityThreshold = 80.0;
  double windThreshold = 15.0;
  double uvThreshold = 7.0;

  @override
  void initState() {
    super.initState();
    loadPrefs();
  }

  Future<void> loadPrefs() async {
    final prefs = await SharedPreferences.getInstance();
    setState(() {
      tempAlert = prefs.getBool('tempAlert') ?? false;
      humidityAlert = prefs.getBool('humidityAlert') ?? false;
      windAlert = prefs.getBool('windAlert') ?? false;
      uvAlert = prefs.getBool('uvAlert') ?? false;
      plantWaterAlert = prefs.getBool('plantWaterAlert') ?? false;

      tempThreshold = prefs.getDouble('tempThreshold') ?? 30.0;
      humidityThreshold = prefs.getDouble('humidityThreshold') ?? 80.0;
      windThreshold = prefs.getDouble('windThreshold') ?? 15.0;
      uvThreshold = prefs.getDouble('uvThreshold') ?? 7.0;
    });
  }

  Future<void> savePrefs() async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('tempAlert', tempAlert);
    await prefs.setBool('humidityAlert', humidityAlert);
    await prefs.setBool('windAlert', windAlert);
    await prefs.setBool('uvAlert', uvAlert);
    await prefs.setBool('plantWaterAlert', plantWaterAlert);

    await prefs.setDouble('tempThreshold', tempThreshold);
    await prefs.setDouble('humidityThreshold', humidityThreshold);
    await prefs.setDouble('windThreshold', windThreshold);
    await prefs.setDouble('uvThreshold', uvThreshold);
  }

  Widget buildSliderSetting({
    required String label,
    required bool enabled,
    required ValueChanged<bool> onToggle,
    required double value,
    required ValueChanged<double> onChanged,
    required double min,
    required double max,
    required int divisions,
    required String unit,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        SwitchListTile(
          title: Text(label),
          value: enabled,
          onChanged: (val) {
            onToggle(val);
            savePrefs();
          },
        ),
        Text("${label.split(" ").last} Threshold: ${value.toStringAsFixed(1)}$unit"),
        Slider(
          value: value,
          min: min,
          max: max,
          divisions: divisions,
          label: "${value.toStringAsFixed(1)}$unit",
          onChanged: (v) {
            onChanged(v);
            savePrefs();
          },
        ),
        const SizedBox(height: 20),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      AlertHelper.setContext(context);
    });
    return Scaffold(
      appBar: AppBar(title: const Text("Settings")),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: ListView(
          children: [
            buildSliderSetting(
              label: "🌞 Enable Temperature Alert",
              enabled: tempAlert,
              onToggle: (v) => setState(() => tempAlert = v),
              value: tempThreshold,
              onChanged: (v) => setState(() => tempThreshold = v),
              min: 20,
              max: 50,
              divisions: 30,
              unit: "°C",
            ),
            buildSliderSetting(
              label: "☔ Enable Humidity Alert",
              enabled: humidityAlert,
              onToggle: (v) => setState(() => humidityAlert = v),
              value: humidityThreshold,
              onChanged: (v) => setState(() => humidityThreshold = v),
              min: 40,
              max: 100,
              divisions: 60,
              unit: "%",
            ),
            buildSliderSetting(
              label: "💨 Enable Wind Alert",
              enabled: windAlert,
              onToggle: (v) => setState(() => windAlert = v),
              value: windThreshold,
              onChanged: (v) => setState(() => windThreshold = v),
              min: 0,
              max: 30,
              divisions: 30,
              unit: " m/s",
            ),
            buildSliderSetting(
              label: "☀️ Enable UV Alert",
              enabled: uvAlert,
              onToggle: (v) => setState(() => uvAlert = v),
              value: uvThreshold,
              onChanged: (v) => setState(() => uvThreshold = v),
              min: 0,
              max: 15,
              divisions: 15,
              unit: "",
            ),
            SwitchListTile(
              title: const Text("🌱 Water the Plants Alert"),
              value: plantWaterAlert,
              onChanged: (val) {
                setState(() {
                  plantWaterAlert = val;
                });
                savePrefs();
              },
            ),
            const SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}
