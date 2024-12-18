import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:latlong2/latlong.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:geolocator/geolocator.dart';

class MapScreen extends StatefulWidget {
  const MapScreen({super.key});

  @override
  _MapScreenState createState() => _MapScreenState();
}

class _MapScreenState extends State<MapScreen> {
  final MapController mapController = MapController();
  List<Marker> markers = [];
  LatLng? _userLocation;

  @override
  void initState() {
    super.initState();
    _getUserLocation(); // Appel de la fonction pour obtenir la localisation de l'utilisateur
    _getTrafficLightsStatus(); // Appel de la fonction pour récupérer les données des feux
  }

  // Fonction pour obtenir la position actuelle de l'utilisateur
  Future<void> _getUserLocation() async {
    Position position = await Geolocator.getCurrentPosition(
      desiredAccuracy: LocationAccuracy.high,
    );
    setState(() {
      _userLocation = LatLng(position.latitude, position.longitude);
    });
  }

  // Fonction pour récupérer l'état des feux depuis l'API Node-RED
  Future<void> _getTrafficLightsStatus() async {
    final response = await http
        .get(Uri.parse('http://192.168.26.200:1880/getTrafficStatus'));

    if (response.statusCode == 200) {
      // Si la requête est réussie, analyser les données
      List<dynamic> data = json.decode(response.body);
      _addTrafficLightMarkers(data); // Ajouter les marqueurs des feux
    } else {
      // Si la requête échoue
      throw Exception('Erreur lors de la récupération des états des feux');
    }
  }

  // Fonction pour ajouter des marqueurs sur la carte pour chaque feu
  void _addTrafficLightMarkers(List<dynamic> trafficLightsData) {
    setState(() {
      markers = trafficLightsData.map<Marker>((lightData) {
        Color markerColor;

        // Déterminer la couleur du marqueur en fonction de l'état des feux
        switch (lightData['color']) {
          case 'red':
            markerColor = Colors.red;
            break;
          case 'green':
            markerColor = Colors.green;
            break;
          case 'yellow':
            markerColor = Colors.yellow;
            break;
          default:
            markerColor = Colors.grey;
        }

        // Extraire la latitude et longitude des données
        var position =
            LatLng(lightData['location']['lat'], lightData['location']['lon']);

        return Marker(
          width: 80.0,
          height: 80.0,
          point: position,
          child: GestureDetector(
            onTap: () {
              _showTrafficLightStatus(lightData); // Afficher l'état du feu lorsqu'on clique
            },
            child: Icon(
              Icons.location_on,
              color: markerColor,
              size: 40.0,
            ),
          ),
        );
      }).toList();
    });
  }

  // Afficher l'état des feux dans un modal
  void _showTrafficLightStatus(Map<String, dynamic> lightData) {
    showModalBottomSheet(
      context: context,
      builder: (context) {
        return Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text('Statut des feux ${lightData['id']}'),
              SizedBox(height: 20),
              _buildTrafficLightStatus('Voie 1', lightData['lane1_status']),
              _buildTrafficLightStatus('Voie 2', lightData['lane2_status']),
            ],
          ),
        );
      },
    );
  }

  // Afficher l'état de chaque voie
  Widget _buildTrafficLightStatus(String lane, String status) {
    IconData icon;
    Color color;

    if (status == 'red') {
      icon = Icons.stop;
      color = Colors.red;
    } else if (status == 'green') {
      icon = Icons.play_arrow;
      color = Colors.green;
    } else if (status == 'yellow') {
      icon = Icons.pause;
      color = Colors.yellow;
    } else {
      icon = Icons.offline_bolt;
      color = Colors.grey;
    }

    return ListTile(
      title: Text('$lane: $status'),
      leading: Icon(icon, color: color),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Carte des Feux de Circulation'),
        backgroundColor: Colors.blue,
      ),
      body: Column(
        children: [
          Expanded(
            child: FlutterMap(
              mapController: mapController,
              options: MapOptions(
                initialCenter: _userLocation ?? LatLng(33.241997, -8.498266), // Utilisation de la localisation de l'utilisateur
                 initialZoom: 13.0,
              ),
              children: [
                TileLayer(
                  urlTemplate:
                      "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
                  subdomains: ['a', 'b', 'c'],
                ),
                MarkerLayer(markers: markers),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
