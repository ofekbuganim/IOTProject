import 'package:flutter/material.dart';
import 'pages/login_page.dart';
import 'pages/realtime_page.dart';
import 'pages/history_page.dart';
import 'pages/forecast_page.dart';
import 'pages/settings_page.dart';
import 'pages/home_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Weather',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const LoginPage(),
      routes: {
        '/login': (context) => const LoginPage(),
        '/home': (context) => const HomePage(), // New home
        '/realtime': (context) => const RealTimePage(), // Optional: still keep if you need direct link
        '/history': (context) => const HistoryPage(),
        '/forecast': (context) => const ForecastPage(),
        '/settings': (context) => const SettingsPage(),
      },
    );
  }
}
