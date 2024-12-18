import 'package:flutter/material.dart';
import 'map_screen.dart'; // Assurez-vous que vous avez bien ce fichier
import 'chat_screen.dart'; // Ajouter l'import pour la nouvelle page ChatScreen

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(
          'Accueil',
          style: TextStyle(fontWeight: FontWeight.bold),
        ),
        actions: [
          IconButton(
            icon: const Icon(Icons.map),
            tooltip: 'Voir la carte',
            onPressed: () {
              // Navigation vers la page MapScreen
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const MapScreen()),
              );
            },
          ),
          IconButton(
            icon: const Icon(Icons.chat),
            tooltip: 'ChatBoot',
            onPressed: () {
              // Navigation vers la page ChatScreen
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ChatScreen()),
              );
            },
          ),
        ],
        backgroundColor: const Color.fromARGB(255, 25, 118, 210), // Couleur de fond de l'AppBar
        elevation: 4, // Légère élévation pour l'ombre
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 20.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              // Image ou bannière
              Container(
                margin: const EdgeInsets.only(bottom: 20),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(15),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.blue.withOpacity(0.1),
                      blurRadius: 10,
                      offset: const Offset(0, 4),
                    ),
                  ],
                ),
                child: ClipRRect(
                  borderRadius: BorderRadius.circular(15),
                  child: Image.asset(
                    'assets/images/image.png', // Ajouter une image dans le dossier assets
                    fit: BoxFit.cover,
                    height: 200,
                    width: double.infinity,
                  ),
                ),
              ),
              // Titre de bienvenue
              const Text(
                'Bienvenue sur l\'application!',
                style: TextStyle(
                  fontSize: 30,
                  fontWeight: FontWeight.bold,
                  color: Colors.blueGrey,
                ),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 10),
              const Text(
                'Explorez les intersections et leur statut en temps réel.',
                style: TextStyle(
                  fontSize: 16,
                  color: Colors.grey,
                ),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 30),
              // Bouton d'action pour voir la carte
              ElevatedButton.icon(
                onPressed: () {
                  // Navigation vers la page MapScreen
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => const MapScreen()),
                  );
                },
                icon: const Icon(Icons.map, size: 24),
                label: const Text(
                  'Voir la carte',
                  style: TextStyle(fontSize: 18),
                ),
                style: ElevatedButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 15, horizontal: 30),
                  backgroundColor: Colors.blue.shade600, // Fond du bouton
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(30),
                  ),
                ),
              ),
              const SizedBox(height: 30),
              // Icônes ou actions supplémentaires (si nécessaire)
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  _buildActionCard(
                    icon: Icons.traffic,
                    label: 'Statut des feux',
                    onTap: () {
                      // Action spécifique
                    },
                  ),
                  _buildActionCard(
                    icon: Icons.settings,
                    label: 'Paramètres',
                    onTap: () {
                      // Action spécifique
                    },
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  // Widget pour les cartes d'action
  Widget _buildActionCard({
    required IconData icon,
    required String label,
    required VoidCallback onTap,
  }) {
    return GestureDetector(
      onTap: onTap,
      child: Column(
        children: [
          CircleAvatar(
            radius: 35,
            backgroundColor: Colors.blueAccent.withOpacity(0.1),
            child: Icon(icon, size: 32, color: Colors.blue),
          ),
          const SizedBox(height: 8),
          Text(
            label,
            style: const TextStyle(
              fontSize: 16, 
              fontWeight: FontWeight.w600,
              color: Colors.blueGrey,
            ),
          ),
        ],
      ),
    );
  }
}
