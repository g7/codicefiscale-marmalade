/*
 * CodiciFiscali - My first marmalade application! With embedded python!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "s3e.h"
#include "IwUI.h"
#include "IwResManager.h"

#include <Python.h>

#include <dirent.h>


PyObject *pName, *pModule, *pDict, *pFunc;
PyObject *pArgs, *pValue;

const char* name;
const char* surname;
const char* sex;
const char* day;
const char* month;
const char* year;
bool isForeign = false;
const char* place;

const char* finalResult;


class CAlertDialogHandler : public CIwUIElementEventHandler
{
public:
	bool HandleEvent(CIwEvent* pEvent)
	{
		if (pEvent->GetID() == IWUI_EVENT_ALERT_DIALOG_BUTTON)
		{
			CIwUIEventAlertDialogButton* pEventButton =
            IwSafeCast<CIwUIEventAlertDialogButton*>(pEvent);
            
			_HandleButtonClick(pEventButton->GetButtonIndex());
			return true;
		}
		return CIwUIElementEventHandler::HandleEvent(pEvent);
	}
    
private:
	void _HandleButtonClick(int16 buttonIndex)
	{
			IwGetUIView()->SetModal(NULL);
			GetElement()->SetVisible(false);
	}
};

// Typedef to make code clearer.
typedef CIwUIElementSignal<CIwUISignal1<CIwUIElement* > > Signal_Element;

class CController : public CIwUIController
{
public:

    CController()
    {
		/* IW_UI slots */
        IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateName, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateSurname, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateDay, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateMonth, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateYear, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdateIsForeign, CIwUIElement*, bool);
		IW_UI_CREATE_VIEW_SLOT2(this, "CController", CController, UpdatePlace, CIwUIElement*, bool);

		IW_UI_CREATE_VIEW_SLOT1(this, "CController", CController, isMale, CIwUIElement*);
		IW_UI_CREATE_VIEW_SLOT1(this, "CController", CController, isFemale, CIwUIElement*);
		IW_UI_CREATE_VIEW_SLOT1(this, "CController", CController, FireCode, CIwUIElement*);
    }

    ~CController()
    {
        IW_UI_DESTROY_VIEW_SLOTS(this)
    }

    void UpdateName(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        name = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();
    }

	void UpdateSurname(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        surname = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();

    }

	void UpdateDay(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        day = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();

    }

	void UpdateMonth(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        month = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();
    }

	void UpdateYear(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        year = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();
    }

	void UpdateIsForeign(CIwUIElement* pCheckbox, bool  boh)
    {
        //Retrieve the value from the Checkbox
        isForeign = IwSafeCast<CIwUICheckbox*>(pCheckbox)->GetChecked();

		// reset place field
		IwSafeCast<CIwUILabel*>(IwGetUIView()->GetChildNamed("Place"))->SetCaption("");

		// Also change the label of the country/municipality textfield
		CIwUILabel* pLabel = IwSafeCast<CIwUILabel*>(IwGetUIView()->GetChildNamed("Foreign"));
		if (!isForeign) {
			pLabel->SetCaption("Comune");
		} else {
			pLabel->SetCaption("Stato");
		}
    }

	void UpdatePlace(CIwUIElement* pTextField, bool acceptedNotCancelled)
    {
        //Retrieve the value from the CIwUITextField
        place = IwSafeCast<CIwUITextField*>(pTextField)->GetCaption();
    }

	void isMale(CIwUIElement* pButton)
	{
		sex = "M";
	}

	void isFemale(CIwUIElement* pButton)
	{
		sex = "F";
	}

	void FireCode(CIwUIElement* pButton)
	{
		/* This invokes the python methods */

		if (pModule != NULL) {
			pFunc = PyObject_GetAttrString(pModule, "calcola_codice"); // get the method
			/* pFunc is a new reference */

			if (pFunc && PyCallable_Check(pFunc)) {
				/* we need to build the argument tuple.
				We are using Py_BuildValue: the first char* describes the type of the objects we
				will pass, for example:
				  "sssb" means: first, second and third parameter are strings, the fourth is a boolean object.
				*/
				PyObject* pArgs = Py_BuildValue("sssssssb", name, surname, day, month, year, sex, place, isForeign);
				/* Now we call the method, and store the result to pValue */
				pValue = PyObject_CallObject(pFunc, pArgs);
				Py_DECREF(pArgs);
				if (pValue != NULL) {
					/* We need to convert the result in a c++ string */
					finalResult = PyString_AsString(pValue);
					Py_DECREF(pValue);
				}
			}
			else {
					if (PyErr_Occurred())
						PyErr_Print();
					fprintf(stderr, "Cannot find function.");
			}
		Py_XDECREF(pFunc);
		}

		/* simple, uh? */

		CIwUIAlertDialog* pAlertDialog = new CIwUIAlertDialog;
		pAlertDialog->SetStyle("<alertDialog>");
    
		char buffer[1024];
		if (finalResult) {
			sprintf(buffer, "Il codice fiscale calcolato e'  %s.", finalResult);
		} else {
			sprintf(buffer, "Impossibile calcolare il codice fiscale. Controllare tutti i campi.");
		}
    
		// Setup label
		pAlertDialog->SetLabelStyle("<label_medium>");
		pAlertDialog->SetLabelCaption(buffer);
    
		// Setup buttons
		pAlertDialog->SetButtonStyle(0, "<button>");
		pAlertDialog->SetButtonCaption(0, "OK");
        
		// Setup handler
		pAlertDialog->AddEventHandler(new CAlertDialogHandler);
    
		// Display alert dialog
		IwGetUIView()->AddElementToLayout(pAlertDialog);
		IwGetUIView()->SetModal(pAlertDialog);

	}

};



// "main" is the S3E entry point
int main(int argc,char *argv[])
{
	/* Initialize python */
	Py_Initialize();

	/* Due to some weirdness, we need to add . (data-ram) to sys.path,
	   otherwise we can't load the module */
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('.')");

	/* Mockup is our module, see mockup.py */
	pName = PyString_FromString("mockup");
	/* import it */
	pModule = PyImport_Import(pName);
	/* free */
	Py_DECREF(pName);

	/* ok, the rest is marmalade/iwui relative. See the CController for how
	call the module's methods. */

    IwUIInit();

    //clear to white
    IwGxSetColClear(0xff, 0xff, 0xff, 0xff);

    CIwUIView* pView = new CIwUIView;
    CIwUIController* pController = new CController;

    IwGetResManager()->LoadGroup("ui.group");

    CIwUIElement* pUI = ((CIwUIElement*)IwGetResManager()->GetResNamed("main", "CIwUIElement"))->Clone();
    pView->AddElement(pUI);
    pView->AddElementToLayout(pUI);

    //Create text input singleton
	new CIwUITextInput;

    //Create softkeyboard

    IwGetUITextInput()->CreateSoftKeyboard();

    //Choose type of soft keyboard

	IwGetUITextInput()->SetEditorMode(CIwUITextInput::eOSReadString);

    // to exit correctly, applications should poll for quit requests
    while(!s3eDeviceCheckQuitRequest())
    {
        IwGxClear();

        pView->Update(20);
        pController->Update();

        pView->Render();

        IwGxFlush();
        IwGxSwapBuffers();

        // S3E applications should yield frequently
        s3eDeviceYield();
    }

    delete pController;
    delete pView;
	delete pUI;

	Py_DECREF(pModule);
	Py_Finalize();

    IwUITerminate();

    return 0;
}
