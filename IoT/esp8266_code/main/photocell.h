#ifndef PHOTOCELL_H
#define PHOTOCELL_H

// Définir le pin auquel la photorésistance est connectée
#define LDR_PIN A0  // A0 est l'exemple, vous pouvez utiliser un autre pin analogique
#define LED_PIN 5 

int readLDR() {
  // Lire la valeur analogique de la photorésistance (0 à 1023)
  int ldrValue = analogRead(LDR_PIN);
  
  return ldrValue;
}
#endif // PHOTOCELL_H
