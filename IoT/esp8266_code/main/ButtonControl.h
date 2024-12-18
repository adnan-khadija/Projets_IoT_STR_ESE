#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "LEDControl.h" // Inclure le fichier LEDControl pour contrôler les feux

#define BUTTON_PIN 0 // Pin du bouton d'urgence
bool buttonInverted = false; // Indique si les feux doivent être inversés
unsigned long lastButtonPressTime = 0; // Temps du dernier appui sur le bouton
const unsigned long DEBOUNCE_DELAY = 200; // Anti-rebond : délai minimal entre deux pressions du bouton

/**
 * @brief Vérifie l'état du bouton d'urgence.
 * Si le bouton est pressé, les feux sont temporairement inversés.
 */
void checkButtonState() {
    static bool lastButtonState = HIGH; // État précédent du bouton
    bool currentButtonState = digitalRead(BUTTON_PIN); // Lecture de l'état actuel du bouton

    // Vérification : changement d'état + délai anti-rebond
    if (lastButtonState == HIGH && currentButtonState == LOW && millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
        lastButtonPressTime = millis(); // Met à jour le temps du dernier appui
        buttonInverted = !buttonInverted; // Inverse l'état des feux

        // Logique pour l'inversion des feux
        Serial.println("Bouton d'urgence activé : inversion des feux");
        setLights(buttonInverted); // Applique l'inversion
        delay(3000); // Maintient l'état inversé pendant 3 secondes
        buttonInverted = false; // Retourne à l'état normal
        setLights(buttonInverted); // Réapplique l'état normal
    }

    lastButtonState = currentButtonState; // Met à jour l'état précédent
}

#endif // BUTTONCONTROL_H
