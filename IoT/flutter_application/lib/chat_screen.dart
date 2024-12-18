import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

class ChatScreen extends StatefulWidget {
  const ChatScreen({super.key});

  @override
  _ChatScreenState createState() => _ChatScreenState();
}

class _ChatScreenState extends State<ChatScreen> {
  final TextEditingController _controller = TextEditingController();
  final List<Map<String, String>> _messages = []; // Store messages with sender info
  bool _isLoading = false; // Track if a message is being sent

  // Method to send a message and get a response from the chatbot
  Future<void> _sendMessage() async {
    if (_controller.text.isNotEmpty) {
      setState(() {
        _messages.add({"sender": "user", "message": _controller.text}); // Add user message
        _isLoading = true; // Show loading indicator
      });

      final response = await _sendToApi(_controller.text); // Call the API

      if (response != null) {
        setState(() {
          _messages.add({"sender": "chatbot", "message": response}); // Add chatbot response
          _isLoading = false; // Hide loading indicator
        });
      }

      _controller.clear(); // Clear the input field
    }
  }

  // Function to send the message to the Flask API
  Future<String?> _sendToApi(String userMessage) async {
    const apiUrl = 'http://127.0.0.1:5000/chat'; // Replace with your local machine IP

    try {
      final response = await http.post(
        Uri.parse(apiUrl),
        headers: {"Content-Type": "application/json"},
        body: json.encode({"user_input": userMessage}),
      );

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        return data['response']; // Get the chatbot response
      } else {
        return "Erreur: Impossible d'obtenir une réponse du chatbot.";
      }
    } catch (e) {
      print('Erreur de connexion: $e');
      return "Erreur de connexion avec le serveur.";
    }
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Chat'),
        backgroundColor: const Color.fromARGB(255, 25, 118, 210),
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Column(
          children: [
            Expanded(
              child: ListView.builder(
                itemCount: _messages.length,
                itemBuilder: (context, index) {
                  final message = _messages[index];
                  final isUserMessage = message['sender'] == 'user';

                  return Align(
                    alignment: isUserMessage ? Alignment.centerRight : Alignment.centerLeft,
                    child: Container(
                      margin: const EdgeInsets.symmetric(vertical: 4.0),
                      padding: const EdgeInsets.symmetric(vertical: 10.0, horizontal: 14.0),
                      decoration: BoxDecoration(
                        color: isUserMessage ? Colors.blueAccent : Colors.grey[300],
                        borderRadius: BorderRadius.circular(12.0),
                      ),
                      child: Row(
                        mainAxisSize: MainAxisSize.min,
                        children: [
                          if (!isUserMessage)
                            Padding(
                              padding: const EdgeInsets.only(right: 8.0),
                              child: CircleAvatar(
                                radius: 16.0,
                                backgroundColor: Colors.green,
                                child: Icon(
                                  Icons.android,
                                  color: Colors.white,
                                  size: 18.0,
                                ),
                              ),
                            ),
                          Expanded(
                            child: Text(
                              message['message']!,
                              style: TextStyle(
                                color: isUserMessage ? Colors.white : Colors.black,
                              ),
                            ),
                          ),
                          if (isUserMessage)
                            Padding(
                              padding: const EdgeInsets.only(left: 8.0),
                              child: CircleAvatar(
                                radius: 16.0,
                                backgroundColor: Colors.blueAccent,
                                child: Icon(
                                  Icons.person,
                                  color: Colors.white,
                                  size: 18.0,
                                ),
                              ),
                            ),
                        ],
                      ),
                    ),
                  );
                },
              ),
            ),
            if (_isLoading) // Show a loading indicator while waiting for response
              Padding(
                padding: const EdgeInsets.all(8.0),
                child: CircularProgressIndicator(),
              ),
            Padding(
              padding: const EdgeInsets.symmetric(vertical: 8.0),
              child: Row(
                children: [
                  Expanded(
                    child: TextField(
                      controller: _controller,
                      decoration: const InputDecoration(
                        hintText: 'Tapez un message...',
                        border: OutlineInputBorder(),
                        contentPadding: EdgeInsets.symmetric(vertical: 10.0, horizontal: 12.0),
                      ),
                    ),
                  ),
                  IconButton(
                    icon: const Icon(Icons.send),
                    onPressed: _sendMessage,
                    iconSize: 30.0,
                    color: Colors.blueAccent,
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
