// HookWatcher.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ISOAutoHookWatcher.h"
#include <map>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include "URLEncode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CHookWatcherApp

BEGIN_MESSAGE_MAP(CISOAutoHookWatcherApp, CWinApp)
END_MESSAGE_MAP()

// CHookWatcherApp construction

CISOAutoHookWatcherApp::CISOAutoHookWatcherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CHookWatcherApp object

CISOAutoHookWatcherApp theApp;

// STRUCTURE that holds a hook's id along with its string name
struct HOOKINFO {
	int hookId;
	WCHAR hook_name[60];
} hook_info;

// ARRAY containing hook ids and their names
HOOKINFO hooks[] = {
	AAHOOK_SET_LOCALE, L"AAHOOK_SET_LOCALE",
	AAHOOK_SET_PRODUCT_NAME, L"AAHOOK_SET_PRODUCT_NAME",
	AAHOOK_FILTER_CUSTOM_MODULE, L"AAHOOK_FILTER_CUSTOM_MODULE",
	AAHOOK_LOGIN, L"AAHOOK_LOGIN",
	AAHOOK_LOGOUT, L"AAHOOK_LOGOUT",
	AAHOOK_CREATE_PROJECT, L"AAHOOK_CREATE_PROJECT",
	AAHOOK_MOVE_PROJECT, L"AAHOOK_MOVE_PROJECT",
	AAHOOK_DELETE_PROJECT, L"AAHOOK_DELETE_PROJECT",
	AAHOOK_MODIFY_PROJECT, L"AAHOOK_MODIFY_PROJECT",
	AAHOOK_PROJECT_WORKFLOW, L"AAHOOK_PROJECT_WORKFLOW",
	AAHOOK_CHECKOUT_PROJECT, L"AAHOOK_CHECKOUT_PROJECT",
	AAHOOK_COPYOUT_PROJECT, L"AAHOOK_COPYOUT_PROJECT",
	AAHOOK_PURGE_PROJECT, L"AAHOOK_PURGE_PROJECT",
	AAHOOK_EXPORT_PROJECT, L"AAHOOK_EXPORT_PROJECT",
	AAHOOK_UPGRADE_PROJECT_TO_RICHPRJ, L"AAHOOK_UPGRADE_PROJECT_TO_RICHPRJ",
	AAHOOK_DOWNGRADE_PROJECT_TO_FOLDER, L"AAHOOK_DOWNGRADE_PROJECT_TO_FOLDER",
	AAHOOK_CREATE_DOCUMENT, L"AAHOOK_CREATE_DOCUMENT",
	AAHOOK_MOVE_DOCUMENT, L"AAHOOK_MOVE_DOCUMENT",
	AAHOOK_DELETE_DOCUMENT, L"AAHOOK_DELETE_DOCUMENT",
	AAHOOK_MODIFY_DOCUMENT, L"AAHOOK_MODIFY_DOCUMENT",
	AAHOOK_CHECKOUT_DOCUMENT, L"AAHOOK_CHECKOUT_DOCUMENT",
	AAHOOK_COPYOUT_DOCUMENT, L"AAHOOK_COPYOUT_DOCUMENT",
	AAHOOK_EXPORT_DOCUMENT, L"AAHOOK_EXPORT_DOCUMENT",
	AAHOOK_CHECKIN_DOCUMENT, L"AAHOOK_CHECKIN_DOCUMENT",
	AAHOOK_PURGE_DOCUMENT_COPY, L"AAHOOK_PURGE_DOCUMENT_COPY",
	AAHOOK_FREE_DOCUMENT, L"AAHOOK_FREE_DOCUMENT",
	AAHOOK_REFRESH_DOC_SERV_COPY, L"AAHOOK_REFRESH_DOC_SERV_COPY",
	AAHOOK_REFRESH_DOCUMENT_COPY, L"AAHOOK_REFRESH_DOCUMENT_COPY",
	AAHOOK_CHANGE_DOC_VERSION, L"AAHOOK_CHANGE_DOC_VERSION",
	AAHOOK_CHANGE_DOC_STATE, L"AAHOOK_CHANGE_DOC_STATE",
	AAHOOK_CREATE_REDLINE_DOC, L"AAHOOK_CREATE_REDLINE_DOC",
	AAHOOK_UPDATE_LINK_DATA, L"AAHOOK_UPDATE_LINK_DATA",
	AAHOOK_DELETE_LINK_DATA, L"AAHOOK_DELETE_LINK_DATA",
	AAHOOK_LOCK_DOCUMENT, L"AAHOOK_LOCK_DOCUMENT",
	AAHOOK_ADD_DOCUMENT_FILE, L"AAHOOK_ADD_DOCUMENT_FILE",
	AAHOOK_DELETE_DOCUMENT_FILE, L"AAHOOK_DELETE_DOCUMENT_FILE",
	AAHOOK_CHANGE_DOCUMENT_FILE, L"AAHOOK_CHANGE_DOCUMENT_FILE",
	AAHOOK_FETCH_MULTIDOCS, L"AAHOOK_FETCH_MULTIDOCS",
	AAHOOK_DELETE_DOCUMENT_EXT, L"AAHOOK_DELETE_DOCUMENT_EXT",
	AAHOOK_DELETE_DOCUMENTS, L"AAHOOK_DELETE_DOCUMENTS",
	AAHOOK_COPY_DOCUMENT_CROSS_DS, L"AAHOOK_COPY_DOCUMENT_CROSS_DS",
	AAHOOK_CREATE_SET, L"AAHOOK_CREATE_SET",
	AAHOOK_ADD_SET_MEMBER, L"AAHOOK_ADD_SET_MEMBER",
	AAHOOK_DELETE_SET_MEMBER, L"AAHOOK_DELETE_SET_MEMBER",
	AAHOOK_VERIFY_VERSION, L"AAHOOK_VERIFY_VERSION",
	AAHOOK_VERIFY_TABLES, L"AAHOOK_VERIFY_TABLES",
	AAHOOK_CREATE_TABLES, L"AAHOOK_CREATE_TABLES",
	AAHOOK_COPY_DOC_ATTRIBUTES, L"AAHOOK_COPY_DOC_ATTRIBUTES",
	AAHOOK_DELETE_USER, L"AAHOOK_DELETE_USER",
	AAHOOK_SET_DOC_FINAL_STATUS, L"AAHOOK_SET_DOC_FINAL_STATUS",
	AAHOOK_DELETE_GROUP, L"AAHOOK_DELETE_GROUP",
	AAHOOK_DELETE_WORKFLOW, L"AAHOOK_DELETE_WORKFLOW",
	AAHOOK_DELETE_STATE, L"AAHOOK_DELETE_STATE",
	AAHOOK_DEL_WORKFLOW_STATE, L"AAHOOK_DEL_WORKFLOW_STATE",
	AAHOOK_DELETE_ENVIRONMENT, L"AAHOOK_DELETE_ENVIRONMENT",
	AAHOOK_INVALIDATE_CACHE, L"AAHOOK_INVALIDATE_CACHE",
	AAHOOK_ACTIVATE_INTERFACE, L"AAHOOK_ACTIVATE_INTERFACE",
	AAHOOK_COPY_DOCUMENTS, L"AAHOOK_COPY_DOCUMENTS",
	AAHOOK_DELETE_USERLIST, L"AAHOOK_DELETE_USERLIST",
	AAHOOK_COPY_DOCUMENTS_CROSS_DS, L"AAHOOK_COPY_DOCUMENTS_CROSS_DS",
	AAHOOK_CREATE_VIEW, L"AAHOOK_CREATE_VIEW",
	AAHOOK_MODIFY_VIEW, L"AAHOOK_MODIFY_VIEW",
	AAHOOK_DELETE_VIEW, L"AAHOOK_DELETE_VIEW",
	AAHOOK_ENUMERATE_VIEWS, L"AAHOOK_ENUMERATE_VIEWS",
	AAHOOK_GET_VIEWCOLUMN_NAME, L"AAHOOK_GET_VIEWCOLUMN_NAME",
	AAHOOK_GEN_SETTING_SET_VALUE, L"AAHOOK_GEN_SETTING_SET_VALUE",
	AAHOOK_USER_SETTING_SET_VALUE, L"AAHOOK_USER_SETTING_SET_VALUE",
	AAHOOK_GROUP_MEMBER_CHANGE, L"AAHOOK_GROUP_MEMBER_CHANGE",
	AAHOOK_APPLICATION_EXIT, L"AAHOOK_APPLICATION_EXIT",
	AAHOOK_OPEN_DOCUMENT, L"AAHOOK_OPEN_DOCUMENT",
	AAHOOK_PRINT_DOCUMENT, L"AAHOOK_PRINT_DOCUMENT",
	AAHOOK_START_APPLICATION, L"AAHOOK_START_APPLICATION",
	AAHOOK_DOC_SEND_MAIL, L"AAHOOK_DOC_SEND_MAIL",
	AAHOOK_LOGIN_DLG, L"AAHOOK_LOGIN_DLG",
	AAHOOK_CREATE_PROJECT_DLG, L"AAHOOK_CREATE_PROJECT_DLG",
	AAHOOK_MODIFY_PROJECT_DLG, L"AAHOOK_MODIFY_PROJECT_DLG",
	AAHOOK_PROJECT_PROPERTY_DLG, L"AAHOOK_PROJECT_PROPERTY_DLG",
	AAHOOK_SELECT_PROJECT_DLG, L"AAHOOK_SELECT_PROJECT_DLG",
	AAHOOK_CREATE_DOCUMENT_DLG, L"AAHOOK_CREATE_DOCUMENT_DLG",
	AAHOOK_SAVE_DOCUMENT_DLG, L"AAHOOK_SAVE_DOCUMENT_DLG",
	AAHOOK_MODIFY_DOCUMENT_DLG, L"AAHOOK_MODIFY_DOCUMENT_DLG",
	AAHOOK_DOCUMENT_PROPERTY_DLG, L"AAHOOK_DOCUMENT_PROPERTY_DLG",
	AAHOOK_SELECT_DOCUMENT_DLG, L"AAHOOK_SELECT_DOCUMENT_DLG",
	AAHOOK_FIND_DOCUMENT_DLG, L"AAHOOK_FIND_DOCUMENT_DLG",
	AAHOOK_DOCUMENT_VERSION_DLG, L"AAHOOK_DOCUMENT_VERSION_DLG",
	AAHOOK_WORKFLOW_DLG, L"AAHOOK_WORKFLOW_DLG",
	AAHOOK_CREATE_SET_DLG, L"AAHOOK_CREATE_SET_DLG",
	AAHOOK_MODIFY_SET_DLG, L"AAHOOK_MODIFY_SET_DLG",
	AAHOOK_USER_SETTINGS_DLG, L"AAHOOK_USER_SETTINGS_DLG",
	AAHOOK_CHECKIN_COMMENT_DLG, L"AAHOOK_CHECKIN_COMMENT_DLG",
	AAHOOK_VIEW_EDITOR_DLG, L"AAHOOK_VIEW_EDITOR_DLG",
	AAHOOK_VIEW_DOCUMENTS, L"AAHOOK_VIEW_DOCUMENTS",
	AAHOOK_VIEW_FILE, L"AAHOOK_VIEW_FILE",
	AAHOOK_SHOW_NOTICE_WND, L"AAHOOK_SHOW_NOTICE_WND",
	AAHOOK_SPLASH_WINDOW, L"AAHOOK_SPLASH_WINDOW",
	AAHOOK_SELECT_REDLINE_DOC_DLG, L"AAHOOK_SELECT_REDLINE_DOC_DLG",
	AAHOOK_START_REDLINE, L"AAHOOK_START_REDLINE",
	AAHOOK_REDLINE_FIND_FILE, L"AAHOOK_REDLINE_FIND_FILE",
	AAHOOK_IMPORTBYDROPHANDLE, L"AAHOOK_IMPORTBYDROPHANDLE",
	AAHOOK_EXEC_MENU_COMMAND, L"AAHOOK_EXEC_MENU_COMMAND",
	AAHOOK_INIT_POPUPMENU, L"AAHOOK_INIT_POPUPMENU",
	AAHOOK_SELECT_INTERFACE_DLG, L"AAHOOK_SELECT_INTERFACE_DLG",
	AAHOOK_PROJECT_EXPORT_WZRD, L"AAHOOK_PROJECT_EXPORT_WZRD",
	AAHOOK_CREATE_DOCUMENTS_DLG, L"AAHOOK_CREATE_DOCUMENTS_DLG",
	AAHOOK_TRANSFER_DOCUMENT_DLG, L"AAHOOK_TRANSFER_DOCUMENT_DLG",
	AAHOOK_SELECT_ENVIRONMENT_DLG, L"AAHOOK_SELECT_ENVIRONMENT_DLG",
	AAHOOK_CODE_RESERVATION_DLG, L"AAHOOK_CODE_RESERVATION_DLG",
	AAHOOK_DOCUMENT_EXPORT_WZRD, L"AAHOOK_DOCUMENT_EXPORT_WZRD",
	AAHOOK_OPEN_DOCUMENTS_DLG2, L"AAHOOK_OPEN_DOCUMENTS_DLG2",
	AAHOOK_SHOW_DOC_PROP_PAGE, L"AAHOOK_SHOW_DOC_PROP_PAGE",
	AAHOOK_SHOW_PROJ_PROP_PAGE, L"AAHOOK_SHOW_PROJ_PROP_PAGE",
	AAHOOK_EXECUTE_DOC_ACTION, L"AAHOOK_EXECUTE_DOC_ACTION",
	AAHOOK_RELOAD_UPDATED_DOCS_DLG, L"AAHOOK_RELOAD_UPDATED_DOCS_DLG",
	AAHOOK_CODE_GENERATION_DLG, L"AAHOOK_CODE_GENERATION_DLG",
	AAHOOK_SAVE_DOCUMENT_DLG3, L"AAHOOK_SAVE_DOCUMENT_DLG3",
	AAHOOK_DOCUMENT_IN_USE_CHECK, L"AAHOOK_DOCUMENT_IN_USE_CHECK",
	AAHOOK_IMPORTBYSTGMEDIUM, L"AAHOOK_IMPORTBYSTGMEDIUM",
	AAHOOK_DLG_APPCHANGED, L"AAHOOK_DLG_APPCHANGED",
	AAHOOK_OPEN_MULTI_DOCUMENTS_DLG, L"AAHOOK_OPEN_MULTI_DOCUMENTS_DLG"
};

// object that maps a hook's id value to the index into the hooks[] array so that we can lookup the name of the hook
std::map <int, int> hookMap;	// hookId => index into hooks

// CHookWatcherApp initialization

BOOL CISOAutoHookWatcherApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
BOOL CheckRootFolder(long leafFolderID)
{
	long status;
	long parentID;
	status = aaApi_SelectParentProject(leafFolderID);
	parentID = aaApi_GetProjectNumericProperty(PROJ_PROP_ID,0);
	LPCWSTR parentName = aaApi_GetProjectStringProperty(PROJ_PROP_NAME,0);
	if(wcscmp(L"Piping Isometrics",parentName) == 0)
	{
		status = aaApi_SelectParentProject(parentID);
		parentID = aaApi_GetProjectNumericProperty(PROJ_PROP_ID,0);
		parentName = aaApi_GetProjectStringProperty(PROJ_PROP_NAME,0);
		if(wcscmp(L"Drawings",parentName)==0)
			return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------+
| CheckTargetFolder - this will look to see if the base target is available.   |
| if not prompt to have the user create the ouput source. Since this will be   |
| invoked from an application it is pointless to create the missing folder.    |
+-----------------------------------------------------------------------------*/
BOOL CheckTargetFolder(long leafFolderID)
{
	long status;
	long parentID;
	status = aaApi_SelectParentProject(leafFolderID);
	parentID = aaApi_GetProjectNumericProperty(PROJ_PROP_ID,0);
	LPCWSTR parentName = aaApi_GetProjectStringProperty(PROJ_PROP_NAME,0);
	if(wcscmp(L"Piping Isometrics",parentName) == 0)
	{
		status = aaApi_SelectParentProject(parentID);
		parentID = aaApi_GetProjectNumericProperty(PROJ_PROP_ID,0);
		parentName = aaApi_GetProjectStringProperty(PROJ_PROP_NAME,0);
		if(wcscmp(L"Drawings",parentName)==0)
		{
			status = aaApi_SelectParentProject(parentID);
		    parentID = aaApi_GetProjectNumericProperty(PROJ_PROP_ID,0);
		    parentName = aaApi_GetProjectStringProperty(PROJ_PROP_NAME,0);
			if (parentID >0)
			{ 
				HAADMSBUFFER buffer;
				buffer = aaApi_SelectProjectDataBufferChilds2(parentID,false);
				int childCount = aaApi_DmsDataBufferGetCount(buffer);
				for (int i = 0;i<childCount; i++)
				{
					LPCWSTR childName = aaApi_DmsDataBufferGetStringProperty(buffer,PROJ_PROP_NAME,i);
					if (wcscmp(childName,L"Sheets") == 0)
					{
						long pid = aaApi_DmsDataBufferGetNumericProperty(buffer,PROJ_PROP_ID,i);
						HAADMSBUFFER b2 = aaApi_SelectProjectDataBufferChilds2(pid,false);
						int subChildCount = aaApi_DmsDataBufferGetCount(b2);
            
						for (int j = 0;j<subChildCount;j++)
						{
							 LPCWSTR childName = aaApi_DmsDataBufferGetStringProperty(b2,PROJ_PROP_NAME,j);

							 if (wcscmp(childName,L"Piping Isometrics")==0)
							 {
								 return true;
							 }
						}
					}
				}//end for childs...
			}//end parentid
		}//end drawings
	}//end piping iso
	return false;
}

// Typically there is one hook function for each hook
// but you can use one hook function and use a switch(hookId) statement to determine which hook is called.
// In our case, one hook function is enough for our purposes.
// Please see the "hookimpl" project in the sample solution provided with the SDK for a sample of how to
// use a hook function in a more typical use case.
LONG AAAPIHOOK  Hook_Watcher
	(
	LONG            hookId,
	LONG            hookType,
	AAPARAM         aParam1,
	AAPARAM         aParam2,
	AARESULT*       pResult
	)
{
	static BOOL bFirstTimeCalled = TRUE;

	if(bFirstTimeCalled)
	{
		bFirstTimeCalled = FALSE;
		TRACE(L"***********************************************************************************\n");
		TRACE(L"********************************* FIRST HOOK CALL *********************************\n");
		TRACE(L"***********************************************************************************\n");
	}
	
	TRACE(L"******************** Hook: %s (%ld)  Value of aParam2: %ld \n", hooks[hookMap[hookId]].hook_name, hookId, aParam2);
	switch (hookId)
	{
		case AAHOOK_LOGIN:
			{
				wchar_t buffer[512];
				int bufferSize = 512;
				if  (aParam2 == AAOPER_USER_LOGIN)
					{
						AALOGIN_PARAM* params = (AALOGIN_PARAM*)aParam1;

						if (aaApi_GetUserStringSetting(AADMS_PAR_WRK_WORKING_DIRECTORY, buffer, bufferSize))
							TRACE(L"the buffer is %S \n",buffer);
						LPCWSTR workingDir;
						workingDir = aaApi_GetWorkingDirectory();
 						TRACE(L"the working dir is %S \n",workingDir);
					}
				break;
			}
			//in case we want to look at the checkin event to update something.
			//I am thinking this will not work for this case.  we only need to 
			//create the ISO one time after that we don't want to recreate the 
			//iso.
		case AAHOOK_CHECKIN_DOCUMENT:
			{
				AADOCUMENTS_PARAM* param = (AADOCUMENTS_PARAM*)aParam1;
				LPAADOC_ITEM lpDoc = param->lpDocuments;

				
				//get the project that is involved.
				//get the document that is being involved
				//queue automation job?
				break;
			}
		case AAHOOK_CREATE_DOCUMENT:
			{
				if(aParam2 == AAOPER_DOC_CREATE)
				{
					AADOCCREATE_PARAM* docParams = (AADOCCREATE_PARAM*)aParam1;
					TRACE(L"creating a new document %S project id %ld doc id %ld \n", docParams->lpctstrFileName,docParams->lProjectId,docParams->lDocumentId);
					//check  the folder structure....
					if(CheckRootFolder(docParams->lProjectId))
					{
						char data[5000];
						HDSOURCE dsHandle=NULL;
						BOOL bODBC;
						long nativeType;
						long loginType;
						wchar_t loginName[AADMS_BUFSIZE_USERNAME];
						wchar_t dsName[AADMS_BUFSIZE_DSNAME];
						wchar_t pwd[AADMS_BUFSIZE_PASSWORD];
						char keyinString[]="sheetautomation create sheet ";
						char pwLoginCMD[]="sheetautomation login";
						char appName[]="SheetAutomation";
						
						CURLEncode encoder;

						aaApi_GetConnectionInfo(dsHandle,&bODBC,&nativeType,&loginType,dsName,AADMS_BUFSIZE_DSNAME,loginName,AADMS_BUFSIZE_USERNAME,pwd,AADMS_BUFSIZE_PASSWORD,NULL,0);
						//if the base target is not there then prompt to create?
						if(!CheckTargetFolder)
						{
						}
						sprintf_s(data,"docset=\"%ld:%ld \"&pwdatasource=%S&pwusername=%S&pwpwd=%S&appKeyin=\"%s\"&appName=%s&pwLoginCMD=\"%s\" ", 
							docParams->lProjectId, docParams->lDocumentId,dsName,loginName,pwd,keyinString,appName,pwLoginCMD);
						


						//string params = data;
						CString c_params(data,sizeof(data));
					    encoder.Encode(c_params);
						CT2CA pzParams (c_params);
						string params(pzParams);

						LONG bufferLen = 1024;
						wchar_t buffer[1024];
						LONG errID;
						LPCWSTR lastMessage;
						LPCWSTR lastDetail;

						if(!aaApi_GetStringSetting(-12471,buffer,& bufferLen))
						{
							errID = aaApi_GetLastErrorId();
							lastDetail = aaApi_GetLastErrorDetail();
							lastMessage = aaApi_GetLastErrorMessage();

								wcscpy_s(buffer,L"http://kaci-dwvusr-01-na.bvcorp.net/");
						}
						wstring url = buffer;
						url.append (L"HPEDocProcessorWebService/HPEDocProcessorWebService.asmx/RunJobEx");

						//sending an encoded string...
						WinHttpClient client(url.c_str());
						//http://kaci-dwvusr-01.na.bvcorp.net/HPEDocProcessorWebService/HPEDocProcessorWebService.asmx
						//(L"http://localhost/HPEDocProcessorWebService/HPEDocProcessorWebService.asmx/RunJobEx");
					
						client.SetAdditionalDataToSend((BYTE *)params.c_str(),params.size());

						 // Set request headers.
						wchar_t szSize[50] = L"";
						swprintf_s(szSize, L"%d", params.size());
						wstring headers = L"Content-Length: ";
						headers += szSize;
						headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
						client.SetAdditionalRequestHeaders(headers);
						wstring fileName = docParams->lpctstrName;
						wstring msg = L"Sent Sheet Create Request - ";
						msg.append(fileName);

						// Send http post request.
						client.SendHttpRequest(L"POST");

						if (client.SendHttpRequest(L"POST"))
							aaApi_ShowInfoMessage(msg.c_str());
						else
						{
							wstring respCode = client.GetResponseStatusCode();
							respCode.append(L" - Error ");
							msg = L"Could not send request for - ";
							msg.append(fileName);
							aaApi_ShowMessage(respCode.c_str(),msg.c_str(),true,false);
							aaApi_ShowErrorMessageDlg(NULL);
						}
						wstring httpResponseHeader = client.GetResponseHeader();
						wstring httpResponseContent = client.GetResponseContent();

					}
					else // not in the target just print out the create happend
					{
						wstring filename = docParams->lpctstrName;
						wstring msg = L"Created File - ";
						msg.append(filename);
						aaApi_ShowInfoMessage(msg.c_str());
					}
				}
				break;
			}
		default:
			{
			}
	}

	// from the ProjectWise V8I Course Guide
	/*
		Setting pResult value
		Always set the value of pResult. This controls how error messages will behave.
		Not all hook ids look at pResult, and not all are documented so set it to be safe.
		If you return AAHOOK_ERROR and you want to display an error, use
		aaApi_SetLastError and aaApiExtendLastError to set up the error information and
		set *pResult = IDABORT. If you don't want an error message if a user chooses to
		cancel operation set *pResult = IDCANCEL.
		If you return AAHOOK_SUCCESS, it is recommended that you set *pResult = IDOK.
	*/
	*pResult = IDOK;

	return AAHOOK_SUCCESS;
}

// This is the function that you use to load your custom module
// Please see the ProjectWise V8i SDK documentation for more information.
extern "C" LONG AAAPI CustomInitialize
	(
	ULONG           ulMask,     // i Application Mask
	LPVOID          lpReserved  // i Reserved (must be NULL)
	)

{
	// uncomment the following line if you use any MFC in this function
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static BOOL bCustInitCalled = FALSE;	// make sure that you set your hooks only once

	if (!bCustInitCalled)
	{
		int count = sizeof(hooks)/sizeof (HOOKINFO);
	//	for (int i=0; i < count; i++)
	//	{
	//		hookMap[hooks[i].hookId] = i;
	//		aaApi_AddHook(hooks[i].hookId, AAPREHOOK, Hook_Watcher);
	//		//aaApi_AddHook(hooks[i].hookId, AAPOSTHOOK, Hook_Watcher);
	//	}
		aaApi_AddHook (AAHOOK_CREATE_DOCUMENT,AAPOSTHOOK, Hook_Watcher);
		//aaApi_AddHook (AAHOOK_CHECKIN_DOCUMENT,AAPOSTHOOK, Hook_Watcher);
		//aaApi_AddHook (AAHOOK_CREATE_DOCUMENT,AAPREHOOK, Hook_Watcher);
		//aaApi_AddHook (AAHOOK_CHECKIN_DOCUMENT, AAPREHOOK, Hook_Watcher);

		bCustInitCalled = TRUE;
	}

	return IDOK;
}
