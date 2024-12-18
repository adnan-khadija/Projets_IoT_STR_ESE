
# Schéma d'Alimentation à Découpage avec Rétroaction et Intégration d'Optocoupleur

## Description du Système

Ce projet concerne la conception d'un schéma d'alimentation à découpage avec rétroaction, destiné à fournir une tension de sortie stable (12V) malgré les variations de la charge. Il inclut l'ajout d'un **optocoupleur** pour assurer une isolation galvanique entre la partie basse tension et la partie haute tension. Ce système est conçu pour être utilisé dans des applications où une régulation fine de la sortie est nécessaire, comme dans les chargeurs et les systèmes d'alimentation pour dispositifs électroniques.

### Fonctionnalités principales :
- **Régulation de la tension de sortie** : Maintien d'une tension stable de 12V malgré les variations de la charge.
- **Isolement galvanique** : Utilisation d'un optocoupleur pour séparer la partie haute tension (secteur 230V AC) et la partie basse tension (commande).
- **Sécurité** : Protection contre les surtensions grâce à une varistance et des filtres EMI.

---

## Schéma du Circuit

![GP2_shema_Flyback_chargeur](https://github.com/user-attachments/assets/51a35553-85a9-45ff-82e8-5a7664c97bea)
![GP2_shemade base_flyback_chargeur](https://github.com/user-attachments/assets/0303fed7-9a57-4cd4-ab7e-229ef36a9558)

### **1. Partie Entrée (Secteur 230VAC) :**
- **V1 (vSINE)** : Source d'alimentation secteur **230VAC**.
- **VR1** (Varistance) : Protège le circuit contre les surtensions en limitant les pics de tension.
- **C4 et L3 (Filtres)** :
   - **C4** : Condensateur pour réduire les perturbations haute fréquence.
   - **L3** : Self pour filtrer les parasites électromagnétiques (EMI), améliorant ainsi la performance du système.
- **R1 et C1** :
   - **R1** : Résistance pour limiter le courant d'appel initial lors de la mise sous tension.
   - **C1** : Filtre supplémentaire pour adoucir le redressement de la tension.

### **2. Redressement et Filtrage :**
- **BR1 (Pont de diodes)** : Redresse la tension AC en tension DC pulsée, permettant une conversion nécessaire pour l'alimentation à découpage.
- **C5** : Condensateur de filtrage qui lisse la tension redressée pour fournir une **haute tension continue** stable, prête à être utilisée par l'oscillateur PWM.

### **3. Oscillateur PWM (Partie basse tension avec NE555) :**
- **BAT1** : Source d'alimentation 20V pour la partie commande.
- **U1 (NE555)** :
   - Génère un **signal PWM** qui est crucial pour contrôler la puissance envoyée au transformateur.
   - Le **rapport cyclique** (duty cycle) du signal PWM détermine l'énergie transférée, régulant ainsi la sortie du système.
- **RV1 (Potentiomètre)** : Permet de moduler la fréquence ou le rapport cyclique du NE555 pour ajuster la puissance fournie au transformateur.
- **R2, R3, R4 et C6** : Ces composants définissent la fréquence de fonctionnement du NE555.
- **Q1 (2N2222) et Q2 (2N2905)** : Transistors de commande qui amplifient le signal PWM, permettant une commande précise du transistor de puissance **Q3**.
- **Optocoupleur U2** :
   - Inséré entre l'oscillateur PWM et la partie de puissance, l'optocoupleur isole électriquement les deux sections.
   - La LED interne de l'optocoupleur est pilotée par le signal PWM généré par **U1 (NE555)**, et la photodiode ou phototransistor de sortie fournit un signal de commande pour **Q3**, le transistor de puissance, tout en assurant une isolation galvanique.

### **4. Commutation et Transformateur :**
- **Q3 (2N3055)** : Transistor de puissance qui découpe la tension continue à haute fréquence. Ce découpage en mode **ON/OFF** contrôle la puissance envoyée au transformateur.
- **TR2 (Transformateur)** :
   - Abaisse la tension continue pulsée à une **tension plus faible** et assure une **isolation galvanique** entre l'entrée (secteur) et la sortie, garantissant ainsi la sécurité et la stabilité du système.

### **5. Redressement et Filtrage secondaire :**
- **D1, D2, D3, D4 (1N4007)** : Diodes qui réalisent un **redressement en pont**, transformant la sortie du transformateur en une tension continue.
- **C2** : Condensateur de filtrage qui lisse la tension redressée, obtenant ainsi une sortie **DC stable**.

### **6. Sortie régulée :**
- **Ampèremètre (A)** : Permet de mesurer le courant délivré en sortie, essentiel pour la surveillance du système.
- **R7, R8, R9, R11** : Résistances qui simulent la consommation sur la sortie pour tester la régulation de la tension.

---

## Fonctionnement global du schéma avec rétroaction (feedback) et ajout de l'Optocoupleur :

1. **Étape d'entrée** : La tension secteur **230VAC** passe d'abord par un dispositif de protection et un filtre EMI, puis elle est redressée et filtrée pour produire une haute tension continue.
2. **Oscillateur PWM (NE555) et Optocoupleur** :
   - Le NE555 génère un signal PWM qui régule la commutation du transistor de puissance **Q3**.
   - L’optocoupleur **U2** est inséré pour isoler galvaniquement les parties à haute et basse tension. La LED interne de l'optocoupleur est activée par le signal PWM, et le phototransistor de sortie contrôle le transistor de puissance **Q3**.
3. **Découpage** : Le transistor **Q3** découpe la tension continue à haute fréquence pour l'alimentation du transformateur.
4. **Transformateur** : Le transformateur abaisse la tension tout en assurant l'isolation galvanique, garantissant la sécurité du système.
5. **Redressement secondaire** : La sortie du transformateur est redressée et filtrée pour obtenir une tension continue stable, prête à être utilisée.
6. **Régulation avec rétroaction et isolation** : Un système de rétroaction ajuste dynamiquement le rapport cyclique du signal PWM, maintenant la tension de sortie stable à 12V malgré les variations de la charge. L'optocoupleur joue un rôle essentiel en isolant la partie de commande, ce qui protège le circuit de puissance contre les perturbations tout en maintenant un contrôle précis et sécurisé de la commutation du transistor de puissance.

---

## Conclusion :
Ce schéma d'alimentation à découpage avec **rétroaction** (feedback) et ajout de l'optocoupleur permet d'obtenir une tension de sortie stable (**12V**) tout en assurant une isolation galvanique entre la section secteur (haute tension) et la section de commande (basse tension). La **rétroaction** et l'optocoupleur garantissent non seulement la stabilité de la tension de sortie, mais aussi la sécurité du système contre les dommages dus aux surtensions et les interférences électromagnétiques.

---


