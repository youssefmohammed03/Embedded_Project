#include "AlarmControlForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread] // Indicates the application uses the single-threaded apartment model
void main() {
    // Enable visual styles for the application
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Create an instance of the form and run it
    Application::Run(gcnew AlarmControlForm());
}
