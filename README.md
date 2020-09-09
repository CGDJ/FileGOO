# File GOO

File GOO is a MIT licenced,  single C++ header file designed to allow quick file manipulation, whilst remaining lightweight and cross platform.

Features:

- Uses only C++ (17) STL with no external dependencies.   
- Create file manifests from directories. 
- Find duplicate files.
- Quickly copy and delete multiple files. 
- Copy all files from a directory (including subfolders), with extension filtering. 
- Copy directory structures without copying files. 
- Extra Debugging info that can be compiled only when required. 



### Example Code:

##### Example 1:

Copy folder structure. 

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
std::filesystem::path toPath = "C:\Sudo\Example\DestinationPath";
    
int main(){
    
    //Copy the full folder structure from one directory to another. 
    CopyFolderStructureOnly(fromPath, toPath);
    
    return 0;
}
```



##### Example 2:

Copy files with extension filters. 

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
std::filesystem::path toPath = "C:\Sudo\Example\DestinationPath";
    
int main(){
    
    //Create a filter list
    std::vector<std::string> filterList;
    fliterList.push_back(".jpg");
    filterList.push_back(".txt");
    filterList.push_back(".png");
    filterList.shrink_to_fit();
    
    //Copy only files that have extensions matching the filter list. 
    CopyFilesWithExtensionFilter(fromPath, toPath, filterList);
    
    return 0;
}
```



##### Example 3:

Create manifest of files in a directory. 

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
    
int main(){
    // container for manifest data
	std::vector<FileGoo_FileDetails> manifest; 
    
    //fill manifest
	FileGoo_AddToManifest(fromPath,manifest);
    
	return 0;
}
```



##### Example 4:

Find duplicates in manifest (This is great if you have several instances of the same file in multiple folders). 

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
    
int main(){
    // container for manifest data
	std::vector<FileGoo_FileDetails> manifest; 
    
    //fill manifest
	FileGoo_AddToManifest(fromPath,manifest);
    
    //create a new manifest with only dulpicates 
    std::vector<FileGoo_FileDetails> duplicates;
	FileGoo_RemoveDulicatesInManifest(test1, duplicates);
       
	return 0;
}
```

##### Example 4:

Copy files in manifest.

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
std::filesystem::path toPath = "C:\Sudo\Example\DestinationPath";
    
int main(){
    // container for manifest data
	std::vector<FileGoo_FileDetails> manifest; 
    
    //fill manifest
	FileGoo_AddToManifest(fromPath,manifest);

    //copy files
    FileGoo_CopyManifestFiles(toPath,manifest)
       
	return 0;
}
```



##### Example 5:

Delete files in manifest. 

###### !!!WARNING!!! ( This removes files from the system you may not be able to recover files once removed, use with caution !!).

```c++
//Example code
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
std::filesystem::path toPath = "C:\Sudo\Example\DestinationPath";

int main(){
    // container for manifest data
	std::vector<FileGoo_FileDetails> manifest; 
    
    //fill manifest
	FileGoo_AddToManifest(fromPath,manifest);
    
    //create a new manifest with only dulpicates 
    std::vector<FileGoo_FileDetails> duplicates;
	FileGoo_RemoveDulicatesInManifest(test1, duplicates);
    
    //Copy files before deletion for saftey 
    FileGoo_CopyManifestFiles(toPath,duplicates)//not needed just for backup
    
    //WARNING!!!
    //Delete all files in duplicates from the system.
    //you may not be able to recover these files, use with caution. 
     FileGoo_DeleteManifestFiles(duplicates);
    //WARNING!!!
    
    
	return 0;
}
```



#### Using verbose console output:

By defining `FILE_GOO_VERBOSE_OUTPUT` before `#INCULDE "CopyManager.h" ` you can easily add helpful console output for debugging. 

##### Verbose example:

```c++
//Example code
#define FILE_GOO_VERBOSE_OUTPUT
#include "FileGOO.h"

// replace with your paths 
std::filesystem::path fromPath = "C:\Sudo\Example\Path\Replace\WithYourPath";
std::filesystem::path toPath = "C:\Sudo\Example\DestinationPath";
    
int main(){
    
    //Create a filter list
    std::vector<std::string> filterList;
    fliterList.push_back(".jpg");
    filterList.push_back(".txt");
    filterList.push_back(".png");
    filterList.shrink_to_fit();
    
    //Copy only files that have extensions matching the filter list. 
    CopyFilesWithExtensionFilter(fromPath, toPath, filterList);
    
    return 0;
}
```

###### **Console output:** 

**`Copied: "someImage.jpg" to "C:\Sudo\Example\DestinationPath"`**

**`Copied: "anotherImage.png" to "C:\Sudo\Example\DestinationPath"`**





#### Know Issues

- Non UTF8 characters may not be displayed/handled correctly. 





#### License: 

Copyright 2020 David Jogoo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.