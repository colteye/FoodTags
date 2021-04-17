# Bill of Materials (Based on current implementation)

## Transmitter

### Diodes
- DSS12UTR diode $0.03468 @ qty 1000
- DSS12UTR diode $0.03468 @ qty 1000

### Thermistor OR Photoresistor
- ERT-J0ER103J thermistor $0.03546 @ qty 1000
- GM5539 photoresistor (cheapest on amazon: $0.08) @ qty 1000

### Inductors
- RLB0812-103KL 10mH inductor $0.15840 @ qty 1000
- RLB0812-103KL 10mH inductor $0.15840 @ qty 1000

### Overall BOM (NOT INCLUDING BOARD AND MANUFACTURING):
$0.46616 @ qty 1000 (Photoresistor) (validated)

$0.42162 @ qty 1000 (Thermistor) (may need additional resistor)

## Reciever

### Diodes
- DSS12UTR diode $0.03468 @ qty 1000
- DSS12UTR diode $0.03468 @ qty 1000

### Inductors
- RLB0812-103KL 10mH inductor $0.15840 @ qty 1000
- RLB0812-103KL 10mH inductor $0.15840 @ qty 1000

### Resistors
- CR0402-FX-4702GLF resistor, 47K $0.00183 @ qty 1000

### Transistors
- MMBT3904-TP transistor $0.01844 @ qty 1000

### Capacitors
- CL05A105KQ5NNNC capacitor, 1µF $0.00509 @ qty 1000

### Microcontroller
- ATTINY85V-15MT MCU $0.50400 @ qty 1000

### Overall BOM (NOT INCLUDING BOARD, BATTERY, AND MANUFACTURING):
$0.91552 @ qty 1000

## Pros

- Very low cost (reciever + transmitter combo could potentially cost less than our original transmitter
- Proven design on the breadboard
- Very few parts (someone could feasibly make the transmitter by themselves without any specialty parts whatsoever)

## Cons

- Only one "sensor" can be used (either a photoresistor, thermistor, etc)
- Power output is quite unstable, not a clean oscillator (can account for this with reciever having software envelope detection)
- Transmitter and reciever need to be extremely close together to get a useful signal

## Notes
- Prices could potentially be further reduced if antennas are made without powerful inductors (which need to be through hole!), instead maybe a less powerful inductor with a custom antenna design.
- rechargable Li-on Battery and support circuitry could potentially get the final reciever price to much higher than currently estimated, but still extremely cheap in comparison to a smartphone (e.g. ~5$)