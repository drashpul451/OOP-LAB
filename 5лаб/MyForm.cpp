#include "MyForm.h"
using namespace Project1;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создаём форму
    MyForm^ form = gcnew MyForm();

    // Запускаем приложение
    Application::Run(form);

    return 0;
}
