//
//  FoodTagsMenuViewController.swift
//  FoodTagsApp
//
//  Created by Erik Coltey on 4/11/21.
//  Copyright © 2021 Erik Coltey. All rights reserved.
//

import UIKit
import CoreNFC
import Charts

/// Struct representing one tag sample.
struct TagSample
{
    var temperature: UInt16
    var illumination: UInt16
    var timestamp: UInt16
}

/// - Tag: FoodTagsMenuViewController
class FoodTagsMenuViewController: UIViewController, NFCNDEFReaderSessionDelegate
{
    /// Instance Variables for NFC Reader Session
    let reuseIdentifier = "reuseIdentifier"
    var detectedMessages = [NFCNDEFMessage]()
    var session: NFCNDEFReaderSession?
    
    /// Returns the temperature in Fahrenheight using the Steinhart and Hart equation from a raw voltage.
    /// Reference: https://create.arduino.cc/projecthub/iasonas-christoulakis/make-an-arduino-temperature-sensor-thermistor-tutorial-b26ed3
    /// - Parameters:
    ///     - voltage: The input voltage between 0 and 1023, with 1023 being Vcc.
    func voltageToTemp(voltage: Double) -> Double
    {
        let R1: Double = 10000
        let c1: Double = 1.009249522e-03
        let c2: Double = 2.378405444e-04
        let c3: Double = 2.019202697e-07

        let R2: Double = R1 * (1023.0 / voltage - 1.0)
        let logR2: Double = log(R2)
        
        /// Steinhart and Hart
        var T: Double = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2))
        
        T = T - 273.15
        T = (T * 9.0) / 5.0 + 32.0
        return T
    }
    
    /// Returns the illuminance in Lux from a raw voltage.
    /// Reference: https://www.digikey.com/en/maker/projects/design-a-luxmeter-with-an-ldr-and-an-arduino/623aeee0f93e427bb57e02c4592567d1
    /// - Parameters:
    ///     - voltage: The input voltage between 0 and 1023, with 1023 being Vcc.
    func voltageToIllum(voltage: Double) -> Double
    {
        let R1: Double = 5100
        let c1: Double = 12518931
        let e1: Double = -1.405
      
        let R2: Double = R1 * (1023.0 / voltage - 1.0)
        let I = c1 * pow(R2, e1);
        return I
    }
    
    /// Function to initialize an NFC NDEF reader session with appropriate error handling.
    @IBAction func beginScanning(_ sender: Any)
    {
        /// Make sure it's possible to read!
        guard NFCNDEFReaderSession.readingAvailable else
        {
            let alertController = UIAlertController(
                title: "Scanning Not Supported",
                message: "This device does not support FoodTag scanning.",
                preferredStyle: .alert
            )
            alertController.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
            self.present(alertController, animated: true, completion: nil)
            return
        }
        
        /// If possible to read, start the session.
        session = NFCNDEFReaderSession(delegate: self, queue: nil, invalidateAfterFirstRead: true)
        session?.alertMessage = "Hold your iPhone near the FoodTag to retrieve freshness data."
        session?.begin()
    }
    
    /// Unnecessary functions (for now).
    func readerSession(_ session: NFCNDEFReaderSession, didInvalidateWithError error: Error) {
        ///
    }
    func readerSessionDidBecomeActive(_ session: NFCNDEFReaderSession) {
        ///
    }

    /// Function executed when Tag Detected and ready to process NFC messages.
    func readerSession(_ session: NFCNDEFReaderSession, didDetectNDEFs messages: [NFCNDEFMessage]) {
        
        /// Instantiate data arrays.
        var samples: [TagSample] = []
        var tempSamples: [Double] = []
        var illumSamples: [Double] = []
        var timestampSamples: [Double] = []
        var id: UInt32 = 0
        
        /// Retrieve all payloads from each NFC message tag provides.
        messages.forEach { (nfcndefMessage) in
          nfcndefMessage.records.forEach({ (nfcndefPayload) in
            
            /// Get data.
            let data = NSData(data: nfcndefPayload.payload)
            
            /// Assign first 4 bytes to the ID variable.
            data.getBytes(&id, length: 4)
            
            for n in 0...23
            {
                /// Convert bytes to a tag sample array.
                var sample = TagSample(temperature: 0, illumination: 0, timestamp: 0)
                data.getBytes(&sample, range: NSMakeRange(4 + (n * 6), 6))
                
                /// Make sure samples are valid!
                if (sample.temperature <= 1024 || sample.illumination <= 1024) {
                    samples.append(sample)
                    
                }
            }
            
            /// BEGIN - Code specifically for Demo using indexes instead of timestamps.
            /// Get the min max values in the tag sample array.
            var max_val = samples.max { $0.timestamp < $1.timestamp }
            var min_val = samples.min { $0.timestamp < $1.timestamp }
           
            /// Check edge case where tag indexes goes from 256 -> 0
            if (min_val?.timestamp == 0 && max_val?.timestamp == UInt16(UINT8_MAX))
            {
                /// Transform timestamps and reacquire min and max values.
                for i in samples.indices
                {
                    samples[i].timestamp = (samples[i].timestamp + 23) % 256
                }
                max_val = samples.max { $0.timestamp < $1.timestamp }
                min_val = samples.min { $0.timestamp < $1.timestamp }
            }

            /// If the minimum value is not 0, make the timestamps go from 0 -> highest.
            if (min_val?.timestamp != 0)
            {
                print(min_val?.timestamp ?? 0)
                for i in samples.indices
                {
                    samples[i].timestamp -= min_val?.timestamp ?? 0
                }
            }
            /// END - Code specifically for Demo using indexes instead of timestamps.
            
            /// Sort samples by their timestamp.
            samples.sort { $0.timestamp < $1.timestamp }
            
            /// For each sample, convert the voltages to temperature/illumination, and add to appropriate arrays.
            for sample in samples
            {
                tempSamples.append(voltageToTemp(voltage: Double(sample.temperature)))
                illumSamples.append(voltageToIllum(voltage: Double(sample.illumination)))
                timestampSamples.append(Double(sample.timestamp))
            }
          })
        }
        
        /// Once finalized, send a new alert!
        session.alertMessage = "FoodTag freshness data retrieved!"
        
        /// Move to the detail page for this tag's data.
        DispatchQueue.main.async{
            let foodTagDataPage = self.storyboard?.instantiateViewController(withIdentifier: "FoodTagData") as! FoodTagsViewController
            
            foodTagDataPage.id = id
            foodTagDataPage.timestampSamples = timestampSamples
            foodTagDataPage.tempSamples = tempSamples
            foodTagDataPage.illumSamples = illumSamples

            self.navigationController?.pushViewController(foodTagDataPage, animated: true)
        }

      }
}
