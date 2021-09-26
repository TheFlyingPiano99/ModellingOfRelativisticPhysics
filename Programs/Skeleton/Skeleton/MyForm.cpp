#include "MyForm.h"


using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    RelativisticPhysics::MyForm form(args);
    Application::Run(% form);
}

System::Void RelativisticPhysics::MyForm::startB_Click(System::Object^ sender, System::EventArgs^ e)
{

    inputHandler.startSym(args);
    return System::Void();
}

System::Void RelativisticPhysics::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    inputHandler.closeSym();
    return System::Void();
}

/*
    RelativisticPhysics::argc = args->Length;

    for (int i = 0; i < RelativisticPhysics::argc; i++) {
        RelativisticPhysics::argv.push_back(mContext.marshal_as<const char*>(args[i]));
    }
*/