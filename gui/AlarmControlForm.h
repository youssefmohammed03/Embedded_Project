#include "alarm.h" // Include the C header file
#include <iostream>
using namespace System;
using namespace System::Windows::Forms;

public ref class AlarmControlForm : public Form {
private:
    Button^ btnToggleAlarm;
    Label^ lblAlarmStatus;

public:
    AlarmControlForm() {
        InitializeComponent();
        AlarmInit(); // Initialize the alarm hardware
    }

private:
    void InitializeComponent() {
        this->Text = "Alarm Control";
        this->Size = System::Drawing::Size(300, 200);

        // Alarm Status Label
        lblAlarmStatus = gcnew Label();
        lblAlarmStatus->Text = "Alarm: Off";
        lblAlarmStatus->Location = System::Drawing::Point(20, 20);
        lblAlarmStatus->Size = System::Drawing::Size(200, 30);
        this->Controls->Add(lblAlarmStatus);

        // Toggle Alarm Button
        btnToggleAlarm = gcnew Button();
        btnToggleAlarm->Text = "Toggle Alarm";
        btnToggleAlarm->Location = System::Drawing::Point(20, 60);
        btnToggleAlarm->Click += gcnew EventHandler(this, &AlarmControlForm::ToggleAlarm);
        this->Controls->Add(btnToggleAlarm);
    }

    void ToggleAlarm(Object^ sender, EventArgs^ e) {
        ToggleAlarm(); // Call the Embedded C function
        UpdateAlarmLabel(); // Update the UI
    }

    void UpdateAlarmLabel() {
        // Read the current alarm state
        uint8_t alarmState = GPIOPinRead(ALARM_PORT, ALARM_PIN); // Use the appropriate GPIO read function
        if (alarmState & ALARM_PIN) {
            lblAlarmStatus->Text = "Alarm: On";
        }
        else {
            lblAlarmStatus->Text = "Alarm: Off";
        }
    }
};
