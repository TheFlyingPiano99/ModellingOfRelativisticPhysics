#include "MyForm.h"
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
    msclr::interop::marshal_context mContext;
    RelativisticPhysics::argc = args->Length;
    
    for (int i = 0; i < RelativisticPhysics::argc; i++) {
        RelativisticPhysics::argv.push_back(mContext.marshal_as<const char*>(args[i]));
    }

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    //WinformCDemo is your project name
    RelativisticPhysics::MyForm form;
    Application::Run(% form);
}

System::Void RelativisticPhysics::MyForm::startB_Click(System::Object^ sender, System::EventArgs^ e)
{

    inputHandler.startSym(argc, &argv[0]);
    return System::Void();
}

System::Void RelativisticPhysics::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    inputHandler.closeSym();
    return System::Void();
}
