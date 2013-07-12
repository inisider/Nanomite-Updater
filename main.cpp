#include <iostream>
#include <windows.h>
#include <set>

using namespace std;

void createFolders(char *path);
void emptyDirectory(char *folderPath);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	bool checkUpdateOfUpdate = true;

	// check if there is updates for update.exe
	if (argc ==2) {
		if (strcmp(argv[1], "update") == 0) {
			CopyFileA("update_tmp.exe", "update.exe", false);

			STARTUPINFOA cif;
			ZeroMemory(&cif, sizeof(STARTUPINFO));
			PROCESS_INFORMATION pi;
			CreateProcessA("update.exe", "skip_update", NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
			return 0;
		} else if (strcmp(argv[1], "skip_update") == 0) {
			checkUpdateOfUpdate = false;
			DeleteFileA("update_tmp.exe");
		}
	}
	
	if (checkUpdateOfUpdate == true) {		
		string updatedFile;

		for (int i = 1; i < argc; i++) {
			updatedFile = argv[i];
			if (updatedFile.find("update.exe") != string::npos) {
				updatedFile = "updates/";
				updatedFile.append(argv[i]);

				CopyFileA(updatedFile.c_str(), "update_tmp.exe", false);
				STARTUPINFOA cif;
				ZeroMemory(&cif, sizeof(STARTUPINFO));
				PROCESS_INFORMATION pi;
				CreateProcessA("update_tmp.exe", "update", NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
				return 0;
			}
		}
	}

	// start of updating debugger
	string updatedFile;
	string existingFile;

	DWORD64 found = 0;

	for (int i = 1; i < argc; i++) {
		existingFile.clear();
		updatedFile.clear();

		updatedFile = argv[i];
		found = updatedFile.find_last_of("/");
	
		existingFile.append("updates/");
		existingFile.append(argv[i]);

 		if (found == string::npos) {
			CopyFileA(existingFile.c_str(), argv[i], false);
		} else {
			createFolders((char *)updatedFile.substr(0, found).c_str());
			CopyFileA(existingFile.c_str(), argv[i], false);
		}
	}

	emptyDirectory("updates/");
	RemoveDirectoryA("updates/");

	// start qtNanomite process
	STARTUPINFOA cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	CreateProcessA("qtNanomite.exe", NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void createFolders(char *path)
{
    char dirName[256];
    char *p = path;
    char *q = dirName;

    while(*p) {
        if (('\\' == *p) || ('/' == *p)) {
            if (':' != *(p - 1)) {
                cout << dirName << "\n";
                CreateDirectoryA(dirName, NULL);
            }
        }

        *q++ = *p++;
        *q = '\0';
    }

    cout << dirName << "\n";
    CreateDirectoryA(dirName, NULL);

//    unsigned found = filePath.find_last_of("/\\");

//    if (found == string::npos) {
//        // there is no folders for creating...
//        cout << "there is no folders for creating...\n";
//        return;
//    }

//    string createFolderName;
//    string path = filePath.substr(0, found);

//    while (1) {
//        found = path.find_first_of("/\\");
//        if (found == string::npos) {
//            break;
//        }

//        createFolderName = path.substr(0, found);
//        path = path.substr(found + 1);

//        // create folder
//        cout << createFolderName.c_str() << "\n";
//    }

//    // create folder
//    cout << path.c_str() << "\n";


////    std::cout << " path: " << str.substr(0,found) << '\n';
////    std::cout << " file: " << str.substr(found+1) << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void emptyDirectory(char* folderPath)
{
 char fileFound[256];
 WIN32_FIND_DATAA info;
 HANDLE hp; 
 sprintf(fileFound, "%s\\*.*", folderPath);
 hp = FindFirstFileA(fileFound, &info);
 do
    {
        if (!((strcmp(info.cFileName, ".")==0)||
              (strcmp(info.cFileName, "..")==0)))
        {
          if((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==
                                     FILE_ATTRIBUTE_DIRECTORY)
          {
              string subFolder = folderPath;
              subFolder.append("\\");
              subFolder.append(info.cFileName);
              emptyDirectory((char*)subFolder.c_str());
              RemoveDirectoryA(subFolder.c_str());
          }
          else
          {
              sprintf(fileFound,"%s\\%s", folderPath, info.cFileName);
              BOOL retVal = DeleteFileA(fileFound);
          }
        }
 
    }while(FindNextFileA(hp, &info)); 
 FindClose(hp);
}
