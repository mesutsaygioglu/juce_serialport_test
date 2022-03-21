#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1200, 250);

	//add comobobox
	portBox.addListener(this);
	addAndMakeVisible(portBox);

	// check the ports and add to the combobox
	const auto portlist(SerialPort::getSerialPortPaths());
	for (int i = 1; i <= portlist.size(); i++)
	{
		//combobox item index starts from 1 not 0, so fix the index here.
		String portName = portlist.getAllValues()[i - 1];

		// this will take only COMX
		portName= portName.getLastCharacters(4);
		portBox.addItem(portName, i);
		DBG("serial port:" << portName << ", size:" << portlist.size()<<", port index : "<<i-1);
	}
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	Rectangle<int> portBoxFrame(120,20);
	Point<int> portBoxCenter(80,20);
	portBoxFrame.setCentre(portBoxCenter);
	portBox.setBounds(portBoxFrame);
}

void MainComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	DBG("comboBoxThatHasChanged");
	if (comboBoxThatHasChanged == & portBox)
	{
		// when we have only one usb port, this index will be zero
		selectedPortIndex = portBox.getSelectedItemIndex();
		DBG("selected item index"<< selectedPortIndex);

		// get port paths
		const auto portlist(SerialPort::getSerialPortPaths());
		DBG("serial port:" << portlist.getAllValues()[selectedPortIndex] << ", size:" << portlist.size());


		DebugFunction theDebugLog;
		if (portlist.size())
		{
			//open the first port on the system
			pSP = std::make_unique <SerialPort> (portlist.getAllValues()[selectedPortIndex],
				                 SerialPortConfig(9600,
												  8,
												  SerialPortConfig::SERIALPORT_PARITY_NONE,
												  SerialPortConfig::STOPBITS_1, 
												  SerialPortConfig::FLOWCONTROL_NONE),
				                 theDebugLog);

			if (pSP->exists())
			{
				DBG("Port is ready");
                pOutputStream = std::make_unique <SerialPortOutputStream> (pSP.get ());
                pInputStream = std::make_unique <SerialPortInputStream> (pSP.get ());

			}
			else
			{
				DBG("Port is NOT ready");
			}
		}
		startTimer(10);
	}
}


void MainComponent::timerCallback()
{
	    //DBG("timer callback");

		// Till this point everything works fine 
		// but after this point it terminate the process 
		// and displays "The thread 0x6a1c has exited with code 0 (0x0)." Then 
		// it keeps jumps into  "pSP does not exist" condition.
		// pSP->exists() keeps returning false.

        if (pSP->exists())
		{
			////or, read line by line:
			String s;
			if (pInputStream->canReadLine())
			{
				s = pInputStream->readNextLine();

				DBG("data" << s);
			}
		}
		else
		{
			DBG("pSP does not exist");
		}
}