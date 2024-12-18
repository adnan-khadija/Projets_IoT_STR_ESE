#ifndef LEDCONTROL_H
#define LEDCONTROL_H

// Définir les broches pour les feux de circulation
#define RED_LIGHT_LANE1 14
#define YELLOW_LIGHT_LANE1 12
#define GREEN_LIGHT_LANE1 13
#define RED_LIGHT_LANE2 4
#define YELLOW_LIGHT_LANE2 5
#define GREEN_LIGHT_LANE2 16
#define LIGHT_SENSOR_PIN A0
// Définir la broche du bouton
#define BUTTON_PIN 0


// extern unsigned long lastUpdateTime;
// extern unsigned long currentDelay;
// extern int currentState;

// bool buttonInverted = false; // Indique si les feux sont inversés (bouton pressé)
// unsigned long lastButtonPressTime = 0; // Temps du dernier appui sur le bouton
// const unsigned long DEBOUNCE_DELAY = 200; // Anti-rebond : délai minimal entre deux pressions du bouton

// /**
//  * @brief Met à jour les feux de circulation en fonction de l'état actuel.
//  * @details Les feux sont inversés si `buttonInverted` est vrai.
//  */
// void setLights() {
//     // Éteindre tous les feux de circulation
//     digitalWrite(RED_LIGHT_LANE1, LOW);
//     digitalWrite(YELLOW_LIGHT_LANE1, LOW);
//     digitalWrite(GREEN_LIGHT_LANE1, LOW);
//     digitalWrite(RED_LIGHT_LANE2, LOW);
//     digitalWrite(YELLOW_LIGHT_LANE2, LOW);
//     digitalWrite(GREEN_LIGHT_LANE2, LOW);

//     // Activer les feux appropriés en fonction de l'état actuel
//     if (!buttonInverted) {
//         switch (currentState) {
//             case 0: // Voie 1 : Vert | Voie 2 : Rouge
//                 digitalWrite(GREEN_LIGHT_LANE1, HIGH);
//                 digitalWrite(RED_LIGHT_LANE2, HIGH);
//                 break;
//             case 1: // Voie 1 : Jaune | Voie 2 : Rouge
//                 digitalWrite(YELLOW_LIGHT_LANE1, HIGH);
//                 digitalWrite(RED_LIGHT_LANE2, HIGH);
//                 break;
//             case 2: // Voie 1 : Rouge | Voie 2 : Vert
//                 digitalWrite(RED_LIGHT_LANE1, HIGH);
//                 digitalWrite(GREEN_LIGHT_LANE2, HIGH);
//                 break;
//             case 3: // Voie 1 : Rouge | Voie 2 : Jaune
//                 digitalWrite(RED_LIGHT_LANE1, HIGH);
//                 digitalWrite(YELLOW_LIGHT_LANE2, HIGH);
//                 break;
//         }
//     } else { // État inversé
//         switch (currentState) {
//             case 0: // Voie 2 : Vert | Voie 1 : Rouge
//                 digitalWrite(GREEN_LIGHT_LANE2, HIGH);
//                 digitalWrite(RED_LIGHT_LANE1, HIGH);
//                 break;
//             case 1: // Voie 2 : Jaune | Voie 1 : Rouge
//                 digitalWrite(YELLOW_LIGHT_LANE2, HIGH);
//                 digitalWrite(RED_LIGHT_LANE1, HIGH);
//                 break;
//             case 2: // Voie 2 : Rouge | Voie 1 : Vert
//                 digitalWrite(RED_LIGHT_LANE2, HIGH);
//                 digitalWrite(GREEN_LIGHT_LANE1, HIGH);
//                 break;
//             case 3: // Voie 2 : Rouge | Voie 1 : Jaune
//                 digitalWrite(RED_LIGHT_LANE2, HIGH);
//                 digitalWrite(YELLOW_LIGHT_LANE1, HIGH);
//                 break;
//         }
//     }
// }

#endif // LEDCONTROL_H
