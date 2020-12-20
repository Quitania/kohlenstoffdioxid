import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'dart:typed_data';
import 'dart:async';
import 'dart:math';
import 'package:usb_serial/usb_serial.dart';
import 'package:usb_serial/transaction.dart';
import 'package:charts_flutter/flutter.dart' as charts;

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  UsbPort _port;
  String _status = "Idle";
  List<Widget> _serialData = [];
  StreamSubscription<String> _subscription;
  Transaction<String> _transaction;
  int _currentValue = 400;
  var _rng = new NotSoRandom();

  Future<bool> _connectTo(device) async {
    _serialData.clear();

    if (_subscription != null) {
      _subscription.cancel();
      _subscription = null;
    }

    if (_transaction != null) {
      _transaction.dispose();
      _transaction = null;
    }

    if (_port != null) {
      _port.close();
      _port = null;
    }

    if (device == null) {
      setState(() {
        _status = "Disconnected";
      });
      return true;
    }

    _port = await device.create();
    if (!await _port.open()) {
      setState(() {
        _status = "Failed to open port";
      });
      return false;
    }

    await _port.setDTR(true);
    await _port.setRTS(true);
    await _port.setPortParameters(
        115200, UsbPort.DATABITS_8, UsbPort.STOPBITS_1, UsbPort.PARITY_NONE);

    _transaction = Transaction.stringTerminated(
        _port.inputStream, Uint8List.fromList([13, 10]));

    _subscription = _transaction.stream.listen((String line) {
      setState(() {
        _serialData.add(Text(line));
        if (_serialData.length > 20) {
          _serialData.removeAt(0);
        }
      });
    });

    setState(() {
      _status = "Connected";
    });
    return true;
  }

  void _getPorts() async {
    List<UsbDevice> devices = await UsbSerial.listDevices();
    print(devices);

    devices.where((device) => "Serpente" == device.productName).forEach((device) {
      _connectTo(device);
    });
  }

  @override
  void initState() {
    super.initState();

    UsbSerial.usbEventStream.listen((UsbEvent event) {
      _getPorts();
    });

    _getPorts();
  }

  @override
  void dispose() {
    super.dispose();
    _connectTo(null);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
          appBar: AppBar(
            title: const Text('O=C=O'),
          ),
          body: Center(
              child: Column(children: <Widget>[
                Text('Status: $_status\n'),
                Text('${"CO\u2082"} level', style: Theme.of(context).textTheme.title),
                Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: Column(
                    children: [
                      new SizedBox(height: 325.0, child: Stack(
                        children: [
                          GaugeChart.fromData(_currentValue),
                          Center(child: Text('$_currentValue', style: Theme.of(context).textTheme.title,))
                        ],
                      )),
                    ],
                  ),
                ),
                kDebugMode ? RaisedButton(onPressed: () => {
                  setState(() => {
                    _currentValue = _rng.nextInt(2000)
                  })
                }, child: Text("Generate new test value"),) : null
              ])),
        ));
  }
}

class GaugeChart extends StatelessWidget {
  final List<charts.Series> seriesList;
  final bool animate;

  GaugeChart(this.seriesList, {this.animate});

  factory GaugeChart.withSampleData() {
    return new GaugeChart(
      _createSampleData(),
      // Disable animations for image tests.
      animate: false,
    );
  }

  factory GaugeChart.fromData(int currentValue) {
    return new GaugeChart(
      _createData(currentValue),
      animate: false,
    );
  }

  @override
  Widget build(BuildContext context) {
    return new charts.PieChart(seriesList,
        animate: animate,
        // Configure the width of the pie slices to 45px. The remaining space in
        // the chart will be left as a hole in the center. Adjust the start
        // angle and the arc length of the pie so it resembles a gauge.
        defaultRenderer: new charts.ArcRendererConfig(
            arcWidth: 45, startAngle: 4 / 5 * pi, arcLength: 7 / 5 * pi));
  }

  static List<charts.Series<GaugeSegment, String>> _createSampleData() {
    final data = [
      new GaugeSegment('Low', 1875, charts.MaterialPalette.green.shadeDefault),
      new GaugeSegment('Acceptable', 1875, charts.MaterialPalette.yellow.shadeDefault),
      new GaugeSegment('High', 1250, charts.MaterialPalette.red.shadeDefault),
    ];

    return [
      new charts.Series<GaugeSegment, String>(
        id: 'Segments',
        domainFn: (GaugeSegment segment, _) => segment.segment,
        measureFn: (GaugeSegment segment, _) => segment.size,
        colorFn: (GaugeSegment segment, _) => segment.color,
        data: data,
      )
    ];
  }

  static List<charts.Series<GaugeSegment, String>> _createData(int currentValue) {
    var data = [
      {'min': 400, 'max': 799, 'size': 1875, 'colour': charts.MaterialPalette.green.shadeDefault, 'segment': 'Low'},
      {'min': 800, 'max': 1399, 'size': 1875, 'colour': charts.MaterialPalette.yellow.shadeDefault, 'segment': 'Medium'},
      {'min': 1400, 'max': 1900, 'size': 1250, 'colour': charts.MaterialPalette.red.shadeDefault, 'segment': 'High'}
    ].map((e) {
      if (currentValue < e['min']) {
        return [new GaugeSegment(e['segment'], e['size'], charts.MaterialPalette.transparent)];
      } else if (currentValue > e['max']) {
        return [new GaugeSegment(e['segment'], e['size'], e['colour'])];
      } else {
        int range = (e['max'] as int) - e['min'];
        int difference = currentValue - e['min'];
        double multiplier = difference / range;
        int colouredNrOfPixels = (multiplier * e['size']).round();
        // partly coloured, rest transparent
        return [new GaugeSegment(e['segment'], colouredNrOfPixels, e['colour']), new GaugeSegment((e['segment'] as String) + '2', (e['size'] as int) - colouredNrOfPixels, charts.MaterialPalette.transparent)];
      }
    }).expand((element) => element).toList();

    return [
      new charts.Series<GaugeSegment, String>(
        id: 'Segments',
        domainFn: (GaugeSegment segment, _) => segment.segment,
        measureFn: (GaugeSegment segment, _) => segment.size,
        colorFn: (GaugeSegment segment, _) => segment.color,
        data: data,
      )
    ];
  }
}

class GaugeSegment {
  final String segment;
  final int size;
  final charts.Color color;

  GaugeSegment(this.segment, this.size, this.color);
}

class NotSoRandom {
  int pointer = 0;
  List<int> list = [405, 525, 550, 575, 600, 650, 700, 800, 1400, 1900, 2000];

  int nextInt(int ignored) {
    pointer++;
    if (pointer > list.length-1) {
      pointer = 0;
    }
    return list[pointer];
  }
}