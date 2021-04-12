//
//  FoodTagsViewController.swift
//  FoodTagsApp
//
//  Created by Erik Coltey on 4/11/21.
//  Copyright © 2021 Erik Coltey. All rights reserved.
//

import UIKit
import Charts

/// - Tag: FoodTagsViewController
class FoodTagsViewController: UIViewController {
    
    /// UI references to the different items that need data.
    @IBOutlet var productID: UILabel!
    @IBOutlet var tempChart: LineChartView!
    @IBOutlet var illumChart: LineChartView!
    
    /// Variables to fill with tag data.
    var id: UInt32 = 0
    var tempSamples: [Double] = []
    var illumSamples: [Double] = []
    var timestampSamples: [Double] = []
    
    /// Set chart data using the relevant values and data points.
    /// - Parameters:
    ///     - chart: LineChartView to reference from UI.
    ///     - dataPoints: Points on X axis.
    ///     - values: Points on Y axis.
    ///     - chartLabel: Legend label for chart line.
    ///     - color: Color of the chart line, circles, and gradient.
    func setChart(chart: LineChartView!, dataPoints: [Double], values: [Double], chartLabel: String, color: NSUIColor) {
        var dataEntries: [ChartDataEntry] = []
                
        /// Fill out data entries.
        for i in 0..<dataPoints.count {
            let dataEntry = ChartDataEntry(x: dataPoints[i], y: values[i])
            dataEntries.append(dataEntry)
        }

        /// Convert entries into dataset.
        let chartDataSet = LineChartDataSet(entries: dataEntries, label: chartLabel)
        
        /// Modify chart aesthetics.
        chartDataSet.mode = .cubicBezier
        chartDataSet.cubicIntensity = 0.2
        chartDataSet.circleRadius = 4.0
        chartDataSet.circleHoleRadius = 0
        chartDataSet.lineWidth = 3.0

        chartDataSet.setColor(color)
        chartDataSet.setCircleColor(color)
        
        /// Gradient under lines.
        let gradientColors = [color.cgColor, UIColor.clear.cgColor] as CFArray
        let colorLocations: [CGFloat] = [1.0, 0.0]
        let gradient = CGGradient.init(colorsSpace: CGColorSpaceCreateDeviceRGB(), colors: gradientColors, locations: colorLocations)
        chartDataSet.fill = LinearGradientFill(gradient: gradient!, angle: 90.0)
        chartDataSet.drawFilledEnabled = true
        chart.data = LineChartData(dataSet: chartDataSet)
        
        /// Make sure border is colored.
        chart.drawBordersEnabled = true
        chart.borderColor = UIColor.systemGray
    }
    
    /// Function when UI view loaded.
    override func viewDidLoad() {
        super.viewDidLoad()
    
        /// When the view is loaded, fill out the UI elements with the appropriate data.
        productID.text = "Product ID: " + String(id)
        
        setChart(chart: tempChart, dataPoints: timestampSamples, values: tempSamples, chartLabel: "Temperature (F)", color: UIColor.systemRed)
        
        setChart(chart: illumChart, dataPoints: timestampSamples, values: illumSamples, chartLabel: "Illuminance (Lux)", color: UIColor.systemBlue)
    }
}
