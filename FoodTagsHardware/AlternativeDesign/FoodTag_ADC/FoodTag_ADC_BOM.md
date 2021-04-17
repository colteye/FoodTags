# Bill of Materials

## Capacitors
- UFW0J102MPD capacitor, 1000µF $0.08419 @ qty 1000
- UFW0J102MPD capacitor, 1000µF $0.08419 @ qty 1000
- GMC21X5R106K16NT capacitor, 10µF $0.01830 @ qty 1000
- GMC21X5R106K16NT capacitor, 10µF $0.01830 @ qty 1000
- CL05A105MQ5NNNC capacitor, 1µF $0.00354 @ qty 1000

## Diodes
- DSS12UTR diode $0.03468 @ qty 1000
- DSS12UTR diode $0.03468 @ qty 1000
- DSS12UTR diode $0.03468 @ qty 1000
- DSS12UTR diode $0.03468 @ qty 1000

## Resistors
- WR04X104 JTL resistor, 100K $0.00134 @ qty 1000
- WR04X103 JTL resistor, 10K $0.00134 @ qty 1000
- WR04X103 JTL resistor, 10K $0.00134 @ qty 1000
- RMCF0402JT5K10 resistor, 5.1K $0.00213 @ qty 1000

## Thermistor
- ERT-J0ER103J thermistor $0.03546 @ qty 1000

## Photoresistor
- Adafruit 161 photoresistor $0.95 @ qty 1000 (potentially as low as 0.08 on amazon)

## ICs
- RF430CL330HCPWR $0.70140 @ qty 1000
- ADS1013IRUGR $0.900 @ qty 1000

## Antenna
- molex 1462360011 $0.34650 @ qty 15000

## PCB (Not flexible, based on EasyEDA estimate)
- FoodTag PCB (JLCPCB) 0.0823 @ qty 1000

## Overall BOM:
$3.36905 @ qty 1000

## Pros
- Lower Cost to initial design
- Should be able to support both sensors
- Should work with smartphones

## Cons
- Not tested
- Likely to not work as the RF430CL330H needs the correct signals to be transmitted to it before sending out NFC data.

## Notes
Price could potentially be further reduced if photoresistor not used (down to ~$2.41905), and microcontroller exchanged for a cheaper one with the same low power properties. May also be of interest to look into digital sensors, however all that fit the ultra low power nature of the project were out of stock at the time.

This version of the tag does require through-hole components (mainly the 1000µF capacitors and the photoresistor).

The Antenna could also be etched onto the PCB directly instead of purchased as a separate component.
