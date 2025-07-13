import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'firebase_options.dart';
import 'pages/home_page.dart';
import 'pages/history_page.dart';
import 'pages/forecast_page.dart';
import 'pages/settings_page.dart';
import 'sensor_data_provider.dart';
import 'utils/alert_helper.dart';
import 'dart:async';
import 'package:connectivity_plus/connectivity_plus.dart';

/// Handles background messages
Future<void> _firebaseMessagingBackgroundHandler(RemoteMessage message) async {
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  print('✅ [Background] Message received: ${message.messageId}');
}

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );

  FirebaseMessaging.onBackgroundMessage(_firebaseMessagingBackgroundHandler);

  runApp(const MyApp());
}

final GlobalKey<NavigatorState> navigatorKey = GlobalKey<NavigatorState>();

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  DateTime? lastReceivedTime;
  bool disconnected = false;
  bool noInternet = false;

  @override
  void initState() {
    super.initState();
    _loadLastReceivedTime();

    SensorDataProvider().stream.listen((data) async {
      final temp = (data['temperature'] ?? data['temperature_outdoor'])?.toDouble() ?? 0;
      final timestamp = data['timestamp'];
      if (temp == 0 || timestamp == null) {
        print('⚠️ Ignoring invalid or empty data');
        return;
      }

      final sensorTime = DateTime.fromMillisecondsSinceEpoch(timestamp.seconds * 1000);

      final prefs = await SharedPreferences.getInstance();
      final stored = prefs.getString('lastReceivedTime');
      final lastSavedTime = stored != null ? DateTime.tryParse(stored) : null;

      // Only update if sensorTime is newer than what we stored
      if (lastSavedTime == null || sensorTime.isAfter(lastSavedTime)) {
        print('✅ Updating lastReceivedTime from sensor timestamp: $sensorTime');
        lastReceivedTime = sensorTime;
        disconnected = false;

        await prefs.setString('lastReceivedTime', sensorTime.toIso8601String());

        AlertHelper.checkAlerts(data);
        setState(() {});
      } else {
        print('⏳ Ignored sensor data with older timestamp: $sensorTime (last saved: $lastSavedTime)');
      }
    });


    // Check for disconnection every 5 seconds
    Timer.periodic(const Duration(seconds: 5), (_) {
      if (lastReceivedTime == null) return;

      final diff = DateTime.now().difference(lastReceivedTime!);
      final shouldBeDisconnected = diff.inSeconds > 30;

      if (shouldBeDisconnected != disconnected) {
        setState(() {
          disconnected = shouldBeDisconnected;
        });
      }
    });

    // ✅ Check internet connectivity
    Connectivity().onConnectivityChanged.listen((ConnectivityResult result) {
      final isOffline = result == ConnectivityResult.none;
      if (isOffline != noInternet) {
        setState(() {
          noInternet = isOffline;
        });
      }
    });
    // ✅ Check current connectivity at startup
    Connectivity().checkConnectivity().then((ConnectivityResult result) {
      final isOffline = result == ConnectivityResult.none;
      if (mounted && isOffline != noInternet) {
        setState(() {
          noInternet = isOffline;
        });
      }
    });
  }

  Future<void> _loadLastReceivedTime() async {
    final prefs = await SharedPreferences.getInstance();
    final saved = prefs.getString('lastReceivedTime');
    if (saved != null) {
      lastReceivedTime = DateTime.tryParse(saved);
      print('🕒 Loaded last update from storage: $lastReceivedTime');
      setState(() {});
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      navigatorKey: navigatorKey,
      title: 'Smart Weather',
      theme: ThemeData.light(),
      darkTheme: ThemeData.dark(),
      themeMode: ThemeMode.system,
      home: Stack(
        children: [
          const MainNavigation(),
          if (disconnected && !noInternet)
            Positioned(
              top: 0,
              left: 0,
              right: 0,
              child: Material(
                elevation: 6,
                color: Colors.red,
                child: SafeArea(
                  child: Container(
                    padding: const EdgeInsets.symmetric(vertical: 10, horizontal: 16),
                    child: Text(
                      '❌ Indoor Station Disconnected — Last update: ${lastReceivedTime?.toLocal().toString().split(".")[0] ?? "unknown"}',
                      style: const TextStyle(
                        color: Colors.white,
                        fontWeight: FontWeight.bold,
                      ),
                      textAlign: TextAlign.center,
                    ),
                  ),
                ),
              ),
            ),
          if (noInternet)
            Positioned(
              top: 30, // Push down if red banner is shown
              right: 10,
              child: Material(
                elevation: 6,
                color: Colors.transparent,
                child: Tooltip(
                  message: 'No Internet Connection',
                  child: Container(
                    decoration: BoxDecoration(
                      color: Colors.orange,
                      shape: BoxShape.circle,
                    ),
                    padding: const EdgeInsets.all(8),
                    child: const Icon(
                      Icons.wifi_off,
                      color: Colors.white,
                      size: 24,
                    ),
                  ),
                ),
              ),
            ),
        ],
      ),
    );
  }
}

class MainNavigation extends StatefulWidget {
  const MainNavigation({super.key});

  @override
  State<MainNavigation> createState() => _MainNavigationState();
}

class _MainNavigationState extends State<MainNavigation> {
  int _selectedIndex = 0;

  final List<Widget> _pages = const [
    HomePage(),
    HistoryPage(),
    ForecastPage(),
    SettingsPage(),
  ];

  void _onTap(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  @override
  void initState() {
    super.initState();
    _setupFirebaseMessaging();
  }

  void _setupFirebaseMessaging() async {
    FirebaseMessaging messaging = FirebaseMessaging.instance;

    NotificationSettings settings = await messaging.requestPermission(
      alert: true,
      badge: true,
      sound: true,
    );

    print('🔔 Permission granted: ${settings.authorizationStatus}');

    String? token = await messaging.getToken();
    print('📲 FCM Token: $token');

    FirebaseMessaging.onMessage.listen((RemoteMessage message) {
      print('📩 [Foreground] Message received: ${message.notification?.title}');
    });

    FirebaseMessaging.onMessageOpenedApp.listen((RemoteMessage message) {
      print('📦 App opened via notification: ${message.notification?.title}');
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: _pages[_selectedIndex],
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _selectedIndex,
        onTap: _onTap,
        type: BottomNavigationBarType.fixed,
        items: const [
          BottomNavigationBarItem(icon: Icon(Icons.sensors), label: "Home"),
          BottomNavigationBarItem(icon: Icon(Icons.show_chart), label: "History"),
          BottomNavigationBarItem(icon: Icon(Icons.cloud), label: "Forecast"),
          BottomNavigationBarItem(icon: Icon(Icons.settings), label: "Settings"),
        ],
      ),
    );
  }
}
