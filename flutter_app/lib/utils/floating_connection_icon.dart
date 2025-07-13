import 'package:flutter/material.dart';
import 'package:connectivity_plus/connectivity_plus.dart';

class FloatingConnectionIcon extends StatefulWidget {
  const FloatingConnectionIcon({super.key});

  @override
  State<FloatingConnectionIcon> createState() => _FloatingConnectionIconState();
}

class _FloatingConnectionIconState extends State<FloatingConnectionIcon> {
  bool isOffline = false;
  late final Connectivity _connectivity;
  late final Stream<ConnectivityResult> _connectivityStream;

  @override
  void initState() {
    super.initState();
    _connectivity = Connectivity();
    _connectivityStream = _connectivity.onConnectivityChanged;

    _connectivityStream.listen((result) {
      setState(() {
        isOffline = result == ConnectivityResult.none;
      });
    });

    _checkInitialConnection();
  }

  Future<void> _checkInitialConnection() async {
    final result = await _connectivity.checkConnectivity();
    setState(() {
      isOffline = result == ConnectivityResult.none;
    });
  }

  @override
  Widget build(BuildContext context) {
    if (!isOffline) return const SizedBox.shrink();

    return Positioned(
      top: MediaQuery.of(context).padding.top + 10,
      right: 10,
      child: Tooltip(
        message: 'No internet connection',
        child: Container(
          padding: const EdgeInsets.all(6),
          decoration: BoxDecoration(
            color: Colors.redAccent,
            shape: BoxShape.circle,
            boxShadow: [
              BoxShadow(
                color: Colors.black26,
                blurRadius: 4,
                offset: Offset(0, 2),
              ),
            ],
          ),
          child: const Icon(
            Icons.wifi_off,
            color: Colors.white,
            size: 20,
          ),
        ),
      ),
    );
  }
}
