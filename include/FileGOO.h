/*
License:

Copyright 2020 David Jogoo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#ifdef FILE_GOO_VERBOSE_OUTPUT
#include <iostream>
#endif // VERBOSE_OUTPUT

//file manifest struct
struct FileGoo_FileDetails {
	
	std::filesystem::path filePath = "";
	std::string fileName = "";
	std::string extenion = "";
	int fileSize = -1; 

	bool compare(FileGoo_FileDetails& other){ 
		if (this->fileName == other.fileName && this->fileSize == other.fileSize) {
			return true;
		}
		else {
			return false;
		}
	
	}
	bool isValid() {
		return std::filesystem::exists(filePath);
	}
	
	
#ifdef FILE_GOO_VERBOSE_OUTPUT
	void printToConsole() {
		//TODO
		//file path from u16string to u8string causes issues with some characters
		//needs a better way to handle certain characters.
		std::cout << "Name = "<<fileName << " : size = " << fileSize << " : path = " << filePath.u8string() << std::endl;
	};
#endif // VERBOSE_OUTPUT
};

//creates a copy of the folder structure 
void CopyFolderStructureOnly(std::filesystem::path sourcePath_, std::filesystem::path destinationPath_) {
	
	// copy options to only create the folders not copy files 
	const auto  copyoptions = std::filesystem::copy_options::directories_only
		| std::filesystem::copy_options::recursive;

	//copy 
	std::filesystem::copy(sourcePath_, destinationPath_ , copyoptions);

};

//Retrives file details about all files in a directory
void FileGoo_AddToManifest(std::filesystem::path sourcePath_, std::vector<FileGoo_FileDetails>& manifest_) {
	
#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "__________________FileGoo_AddToManifest______________________" << std::endl;
	int filecounter = 0;
#endif // VERBOSE_OUTPUT
	
	
	
	//TODO
	//std::filesystem reports error on some paths that include characters.
	//using u16string() for paths and  u8string() for names seems to resolve issue but may cause the path to be unreadabe later 
	//further tests needed. potentially needs some way to detect certain characters and apply a 
	//converion accordingly or change std::locale but for now this seems to work ok. 
	FileGoo_FileDetails info;
	for (auto& file : std::filesystem::recursive_directory_iterator(sourcePath_)) {
		if (file.is_regular_file()) {
			// get file info 
			
			info.filePath = file.path().u16string();
			info.fileName = file.path().filename().u8string();
			info.extenion = file.path().extension().u8string();
			info.fileSize = int(file.file_size());
			//add to our vector 
			manifest_.emplace_back(info);

			//Print to console if verbose output is required.  
#ifdef FILE_GOO_VERBOSE_OUTPUT
			info.printToConsole();
			filecounter++;
#endif // VERBOSE_OUTPUT
		}
	}

#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "___Number of files added to manifest: "<<filecounter<<" _____________________________________________________________" << std::endl;
#endif // VERBOSE_OUTPUT

};

/*Finds files in the first manifest that exist in the second*/
void FileGoo_FindInCommonManifests(std::vector<FileGoo_FileDetails>& manifest_,
										   std::vector<FileGoo_FileDetails>& comparision_manifest_, 
										   std::vector<FileGoo_FileDetails>& result_manifest_){

#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "__________________FileGoo_FindInCommonManifests______________________" << std::endl;
	int filecounter = 0;
#endif // VERBOSE_OUTPUT
	
	for (auto& file : manifest_) {
		for (auto& compfile : comparision_manifest_) {
			if (file.compare(compfile)) {
				// same file found 
				result_manifest_.emplace_back(file);



#ifdef FILE_GOO_VERBOSE_OUTPUT
				std::cout << "Duplicate found: " << file.fileName << std::endl;
				filecounter++;
#endif // FILE_GOO_VERBOSE_OUTPUT


			}
		}
	}


#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "____Number of duplicates found: " << filecounter << " ___________________________________________" << std::endl;
#endif // FILE_GOO_VERBOSE_OUTPUT

};

void FileGoo_RemoveDulicatesInManifest(std::vector<FileGoo_FileDetails>& manifest_, std::vector<FileGoo_FileDetails>& duplicates_) {
	
#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "_______________________FileGOO_RemoveDuplicatesInManifest__________________________________"<< std::endl;
	int dupsfound = 0;
#endif // FILE_GOO_VERBOSE_OUTPUT

	//faster forward looking check
	for (int i = 0; i < manifest_.size(); i++) {
		for (int j = i + 1; j < manifest_.size();j++) {
			if(manifest_.at(i).compare(manifest_.at(j))){
				duplicates_.emplace_back(manifest_.at(i));
				manifest_.erase(manifest_.begin() + i);

#ifdef FILE_GOO_VERBOSE_OUTPUT
				std::cout << "Duplicate found: "<< manifest_.at(i).fileName << std::endl;
				dupsfound++;
#endif // FILE_GOO_VERBOSE_OUTPUT

				break;
			}
		
		}
	
	}
	manifest_.shrink_to_fit();

#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout <<"____Number of duplicates found: "<< dupsfound<<" ___________________________________________" << std::endl;
#endif // FILE_GOO_VERBOSE_OUTPUT


}

//Warning!! removes files in the manifest from the system you may not be able undo this!!!
void FileGoo_DeleteManifestFiles(std::vector<FileGoo_FileDetails>& manifest_) {
#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "_________________FileGoo_DeleteManifestFiles________________" << std::endl;
#endif // FILE_GOO_VERBOSE_OUTPUT

	for (auto& file : manifest_) {
		if (file.isValid()) {

#ifdef FILE_GOO_VERBOSE_OUTPUT
			std::cout << "Removed!: ";
			file.printToConsole();
#endif // FILE_GOO_VERBOSE_OUTPUT

			std::filesystem::remove(file.filePath);
		}
	}

#ifdef FILE_GOO_VERBOSE_OUTPUT
	std::cout << "________________________________________________________" << std::endl;
#endif // FILE_GOO_VERBOSE_OUTPUT
}

//Copy files found in a given manifest to a given location 
void FileGoo_CopyManifestFiles(std::filesystem::path copyDestinationPath_, std::vector<FileGoo_FileDetails>& manifest_, std::filesystem::copy_options copyOptions_ = std::filesystem::copy_options::skip_existing) {
	for (auto& file : manifest_) {
		if (file.isValid()) {
			std::filesystem::copy(file.filePath, copyDestinationPath_, copyOptions_);
		}
	}
}

/*
Copies only files with matching extensions.
Or copy all files except those in filter list if includeFilters_ = false.

example of how to create filter:

std::vector<std::string> filters;
	filters.push_back(".jpg");
	filters.push_back(".JPG");
	filters.push_back(".png");
	filters.shrink_to_fit();

*/
void FileGoo_CopyManifestFiles(std::filesystem::path copyDestinationPath_, std::vector<FileGoo_FileDetails>& manifest_, std::vector<std::string>filters_, bool includeFilters_ = true, std::filesystem::copy_options copyOptions_ = std::filesystem::copy_options::skip_existing){

	for (auto& file : manifest_) {
		for (auto& filter : filters_) {
			if (file.extenion == filter) {

				if (file.isValid()) {
					std::filesystem::copy(file.filePath, copyDestinationPath_, copyOptions_);



#ifdef FILE_GOO_VERBOSE_OUTPUT
					std::cout << "Copied: " << file.fileName << "  to: " << copyDestinationPath_ << std::endl;
#endif // VERBOSE_OUTPUT

				}
				break;
			}

			if (!includeFilters_) {

				if (file.isValid()) {
					std::filesystem::copy(file.filePath, copyDestinationPath_, copyOptions_);


#ifdef FILE_GOO_VERBOSE_OUTPUT
					std::cout << "Copied: " << file.fileName << "  to: " << copyDestinationPath_ << std::endl;
#endif // VERBOSE_OUTPUT

					break;
				}

			}
		}
	}
}

/*
Copies files with matching extensions from given path to destination path.
Or copy all files except those in filter list if includeFilters_ = false.

example of how to create filter:

std::vector<std::string> filters;
	filters.push_back(".jpg");
	filters.push_back(".JPG");
	filters.push_back(".png");
	filters.shrink_to_fit();

*/
void CopyFilesWithExtensionFilter(std::filesystem::path sourcePath_, std::filesystem::path destinationPath_, std::vector<std::string>filters_, bool includeFilters_ = true, std::filesystem::copy_options copyOptions_ = std::filesystem::copy_options::skip_existing) {

	//Set copy options 
	const auto  copyoptions = copyOptions_;

	// temp string init
	std::string ext;



	// itterate over all files in directory 
	for (auto& file : std::filesystem::recursive_directory_iterator(sourcePath_)) {

		// check it is actually a file
		if (file.is_regular_file()) {

			// set our temp string to the current files extention
			ext = file.path().extension().string();

			// iterate throgh filter list, if the file has the same extention as the filter list then copy it
			for (auto& extFilter : filters_) {

				if (ext == extFilter) {

					//copy
					std::filesystem::copy(file.path(), destinationPath_, copyoptions);

#ifdef FILE_GOO_VERBOSE_OUTPUT
					std::cout << "Copied: " << file.path().filename() << "  to: " << destinationPath_ << std::endl;
#endif // VERBOSE_OUTPUT

					// break out of the filter list for loop 
					break;
				}

				//copy if we are using filter list to exclude files
				if (!includeFilters_) {

					//copy
					std::filesystem::copy(file.path(), destinationPath_, copyoptions);

#ifdef FILE_GOO_VERBOSE_OUTPUT
					std::cout << "Copied: " << file.path().filename() << "  to: " << destinationPath_ << std::endl;
#endif // VERBOSE_OUTPUT

					// break out of the filter list for loop 
					break;
				}


			}

		}


	}


};

/*warning this will delete files from the system use with caution!
copies files in the manifest to destination then deletes the original files. 
if copy fails you may lose the file so use with caution. 
*/
void FileGoo_MoveManifestFiles(std::filesystem::path copyDestinationPath_, std::vector<FileGoo_FileDetails>& manifest_, std::filesystem::copy_options copyOptions_ = std::filesystem::copy_options::skip_existing) {

	FileGoo_CopyManifestFiles(copyDestinationPath_, manifest_, copyOptions_);
	//warning deletes files 
	FileGoo_DeleteManifestFiles(manifest_);
};


//TODO
// fix unicode conversion for read write of u16 characters 


//write manifest to file 
void FileGoo_WriteManifestTofile(std::vector<FileGoo_FileDetails>& manifest_, std::filesystem::path file_) {

	std::ofstream outputFile(file_);
	outputFile << "Number of files: " << manifest_.size() << "\n";
	for (auto& file : manifest_) {
		outputFile << file.fileName << "\n" << file.filePath.u8string() << "\n"
			<< file.fileSize << "\n";

	}
	outputFile.close();

};


//Experimental(may fail if reading non u8 characters) Read a manifest from file. 
void FileGoo_ReadManifestFromfile(std::vector<FileGoo_FileDetails>& manifest_, std::filesystem::path file_) {
	
	std::ifstream inputFile;
	inputFile.open(file_);
	if (inputFile.is_open()) {
		FileGoo_FileDetails details;
		std::string temp;
		std::getline(inputFile,temp);
		
		while (inputFile >> details.fileName >> details.filePath >> details.fileSize) {
			manifest_.emplace_back(details); 
		}
		inputFile.close();
	}

};