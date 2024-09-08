#pragma once
#include <JuceHeader.h>
/*
// copyright 2023 Benjamin Smart

// Teddy notes:

// Ryan notes:
 confirmation popup with do it button
 do it button only applies to color coded tags and he keeps forgetting this
requests 'Embed current peak volume' feature
 
// changes in this version
- added padding to first 9 files, so they line up better in the file list.
- added the ability to embed the current peak volume into the Genre metadata field.  if you change the gain of a sample in myriad or sample manager while it's loaded up in this program, you'll have to reload the sample for it to see the new gain.
- added confirmation dialogue box for export samples
- added some colors around the do it button.
 
 
 
// features to suggest:
 
 // make the row that is displayed on the right a darker shade of blue
// we absolutely have to clear this metadata before selling samples with this stuff embedded
 // I think it overwrites files with the same name, should make this add " copy" to the end of the new file, or at least warn.
 
// thoroughly test it to make sure nothing fucks up
// embed parent folder name button
// give a reference list of all the metadata keys i'm using
 
 // confirmation dialogue to prevent bad errors?
// "Do it" button only applies the checkbox color coded tags.  Call this button instead "Apply color tags" or something?
// Undo seems like it would be helpful, I'm not sure how hard to implement
 
 
 Trimmed - riffInfo Artist
 
     
 riffInfoArtist, "Trimmed"); }

riffInfoCopyright, "Faded"); }

 riffInfoKeywords, "RXed"); }

 riffInfoArchivalLocation, "Finalized");

iffInfoCommissioned, custom 1

 riffInfoComment2 custom 2

 
 riffInfoProductName, kitName
 
riffInfoMedium, original filename

riffInfoGenre  current peak gain
 
 
 
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent()
        : lookAndFeel(new juce::LookAndFeel_V3()),
          loadedFilesListBox("Loaded Files", loadedFilesListBoxModel.get()),
          dragAndDropTarget(*this)
    {
        setAudioChannels (0, 0);
        formatManager.registerBasicFormats();

        setSize (900, 600);
        setLookAndFeel(lookAndFeel.get());
        
        addAndMakeVisible(metadataDisplay);
        addAndMakeVisible(systemMessagesDisplay);
        metadataDisplay.setReadOnly (false);
        metadataDisplay.setMultiLine(true);
        metadataDisplay.setWantsKeyboardFocus(true);
        systemMessagesDisplay.setReadOnly (true);
        systemMessagesDisplay.setMultiLine(true);
        systemMessagesDisplay.setWantsKeyboardFocus(false);
        
        
        
        addAndMakeVisible(loadedFilesListBox);
        
        addAndMakeVisible(dragAndDropTarget);
        
        addAndMakeVisible(loadButton);
        addAndMakeVisible(clearButton);
        addAndMakeVisible(doItButton);
        doItButton.setAlpha(0.8f);
        
        addAndMakeVisible(customTag1Button);
        addAndMakeVisible(customTag2Button);
        
        addAndMakeVisible(customTag1TextEditor);
        addAndMakeVisible(customTag2TextEditor);
        
        addAndMakeVisible(editOnlySelectedButton);
        editOnlySelectedButton.setToggleState(true, juce::dontSendNotification);
        
        addAndMakeVisible(trimmedToggleButton);
        addAndMakeVisible(fadedToggleButton);
        addAndMakeVisible(rxedToggleButton);
        addAndMakeVisible(finalizedToggleButton);
        
        addAndMakeVisible(trimmedFilterToggleButton);
        addAndMakeVisible(fadedFilterToggleButton);
        addAndMakeVisible(rxedFilterToggleButton);
        addAndMakeVisible(finalizedFilterToggleButton);

        addAndMakeVisible(exportButton);
        
        addAndMakeVisible(kitNameButton);
        addAndMakeVisible(kitNameTextEditor);
        
        addAndMakeVisible(customFilterToggleButton);
        addAndMakeVisible(customFilterTextEditor);
        customFilterToggleButton.setWantsKeyboardFocus(false);
        
        addAndMakeVisible(currentPeakVolumeButton);
        addAndMakeVisible(originalFilenameButton);

        addAndMakeVisible(exportCSVButton);

        kitNameButton.onClick = [this] { kitNameButtonClicked(); };
        originalFilenameButton.onClick = [this] { originalFilenameButtonClicked(); };
        currentPeakVolumeButton.onClick = [this] { currentPeakVolumeButtonClicked(); };
        
        
        loadButton.onClick = [this] { loadButtonClicked(); };
        clearButton.onClick = [this] { clearButtonClicked(); };
//        clearSelectedButton.onClick = [this] { clearSelectedButtonClicked(); };
                      
        
        doItButton.onClick = [this] { doItButtonClicked(); };
        
        customTag1Button.onClick = [this] { customTag1ButtonClicked(); };
        customTag2Button.onClick = [this] { customTag2ButtonClicked(); };
        
        exportButton.onClick = [this]     { exportButtonClicked(); };
        
        exportCSVButton.onClick = [this]  { exportCSVButtonClicked(); };
        
        editOnlySelectedButton.onClick = [this] { editOnlySelectedEnabled = editOnlySelectedEnabled ? 0 : 1; };
        
        trimmedToggleButton.onClick = [this] { trimmedToggleEnabled = trimmedToggleEnabled ? 0 : 1; };
        fadedToggleButton.onClick = [this] { fadedToggleEnabled = fadedToggleEnabled ? 0 : 1; };
        rxedToggleButton.onClick = [this] { rxedToggleEnabled = rxedToggleEnabled ? 0 : 1; };
        finalizedToggleButton.onClick = [this] { finalizedToggleEnabled = finalizedToggleEnabled ? 0 : 1; };
        
        trimmedFilterToggleButton.onClick = [this]   { trimmedFilterToggleEnabled   = trimmedFilterToggleEnabled ? 0 : 1;   updateLoadedFilesListBox(); };
        fadedFilterToggleButton.onClick = [this]     { fadedFilterToggleEnabled     = fadedFilterToggleEnabled ? 0 : 1;     updateLoadedFilesListBox(); };
        rxedFilterToggleButton.onClick = [this]      { rxedFilterToggleEnabled      = rxedFilterToggleEnabled ? 0 : 1;      updateLoadedFilesListBox(); };
        finalizedFilterToggleButton.onClick = [this] { finalizedFilterToggleEnabled = finalizedFilterToggleEnabled ? 0 : 1; updateLoadedFilesListBox(); };
        
        customFilterToggleButton.onClick = [this] { customFilterToggleEnabled = customFilterToggleEnabled ? 0 : 1; updateLoadedFilesListBox(); };
        
        customFilterTextEditor.onTextChange = [this] { customFilterTextEditorChanged();  };
        
        
        loadedFilesListBox.setMultipleSelectionEnabled(true);
        
        editOnlySelectedButton.setWantsKeyboardFocus(false);
        trimmedToggleButton.setWantsKeyboardFocus(false);
        fadedToggleButton.setWantsKeyboardFocus(false);
        rxedToggleButton.setWantsKeyboardFocus(false);
        finalizedToggleButton.setWantsKeyboardFocus(false);
        trimmedFilterToggleButton.setWantsKeyboardFocus(false);
        fadedFilterToggleButton.setWantsKeyboardFocus(false);
        rxedFilterToggleButton.setWantsKeyboardFocus(false);
        finalizedFilterToggleButton.setWantsKeyboardFocus(false);
                
    }

    ~MainComponent() override
    {
        setLookAndFeel(nullptr);
        shutdownAudio();
    }
    
    // Button callbacks
    
    void loadButtonClicked()
    {
        juce::FileChooser chooser {"Select a directory which contains your samples...", {}, "*.wav"};
        
        if (chooser.browseForDirectory())
        {
            juce::File directory = chooser.getResult();

            if (!directory.exists() || directory.existsAsFile())
            {
                systemMessagesDisplay.insertTextAtCaret(juce::String("directory doesn't exist, or somehow you loaded a file, not a folder") + juce::newLine);
                return;
            }
            
            juce::Array<juce::File> arrayOfFiles = directory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
            juce::StringArray files;
            
            for (int i = 0; i < arrayOfFiles.size(); i++)
            {
                files.add(arrayOfFiles.getUnchecked(i).getFullPathName());
            }
            
            loadSamples(files);
        }
    }
    
    void clearButtonClicked()
    {
        if (editOnlySelectedEnabled)
            clearSelectedFiles();
        else
            clearLoadedFiles();
    }
    
    
    void doItButtonClicked()
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
//        int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to overwrite the embedded color-coded tags?");
//        if (result != 1)
//            return;
        
        if (trimmedToggleEnabled) { setSpecificMetadata(juce::WavAudioFormat::riffInfoArtist, "Trimmed"); }
        else { removeSpecificMetadata(juce::WavAudioFormat::riffInfoArtist); }

        if (fadedToggleEnabled) { setSpecificMetadata(juce::WavAudioFormat::riffInfoCopyright, "Faded"); }
        else { removeSpecificMetadata(juce::WavAudioFormat::riffInfoCopyright); }

        if (rxedToggleEnabled) { setSpecificMetadata(juce::WavAudioFormat::riffInfoKeywords, "RXed"); }
        else { removeSpecificMetadata(juce::WavAudioFormat::riffInfoKeywords); }

        if (finalizedToggleEnabled) { setSpecificMetadata(juce::WavAudioFormat::riffInfoArchivalLocation, "Finalized"); }
        else { removeSpecificMetadata(juce::WavAudioFormat::riffInfoArchivalLocation); }

        repaint();
    }
    
    void customTag1ButtonClicked()
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
        auto customText = customTag1TextEditor.getText().toStdString();
        
        if (!customText.empty())
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to embed Custom Tag 1?");
//            if (result != 1)
//                return;
            setSpecificMetadata(juce::WavAudioFormat::riffInfoCommissioned, customText);
        }
        else
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to remove Custom Tag 1?");
//            if (result != 1)
//                return;
            removeSpecificMetadata(juce::WavAudioFormat::riffInfoCommissioned);
        }
    }
    void customTag2ButtonClicked()
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
        auto customText = customTag2TextEditor.getText().toStdString();
        
        if (!customText.empty())
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to embed Custom Tag 1?");
//            if (result != 1)
//                return;
            setSpecificMetadata(juce::WavAudioFormat::riffInfoComment2, customText);
        }
        else
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to remove Custom Tag 1?");
//            if (result != 1)
//                return;
            removeSpecificMetadata(juce::WavAudioFormat::riffInfoComment2);
        }
    }
    
    void kitNameButtonClicked()
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        

        auto kitName = kitNameTextEditor.getText().toStdString();
        
        if (!kitName.empty())
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", juce::String("Are you sure you want to embed the Kit Name: " + kitName));
//            if (result != 1)
//                return;
            setSpecificMetadata(juce::WavAudioFormat::riffInfoProductName, kitName);
        }
        else
        {
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to remove the Kit Name?");
//            if (result != 1)
//                return;
            removeSpecificMetadata(juce::WavAudioFormat::riffInfoProductName);
        }
    }
    
    void exportButtonClicked()
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
        juce::FileChooser chooser {"Choose target directory to export your samples", {}, ""};
                
        if (chooser.browseForDirectory())
        {
            juce::File targetDirectory = chooser.getResult();
            
//            int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::NoIcon, "", "Are you sure you want to export your files?");
//            if (result != 1)
//                return;
             
            if (editOnlySelectedEnabled)
            {
                auto selectedRows = loadedFilesListBox.getSelectedRows();
                 
                 for (auto i = 0; i < selectedRows.size(); ++i)
                 {
                     auto rowIndex = selectedRows[i];
                    
                     auto filteredFilesRowIndex = filteredFilesIndices[rowIndex];
                     
                     juce::File& file = currentlyLoadedFiles.getReference(filteredFilesRowIndex);
                     
                     auto newFileName = file.getFileName();
                     auto newFilePath = targetDirectory.getChildFile(newFileName);
                     file.copyFileTo(newFilePath);
                 }
            }
            else
            {
                for (juce::File& file : currentlyLoadedFiles)
                {
                    auto newFileName = file.getFileName();
                    auto newFilePath = targetDirectory.getChildFile(newFileName);
                    file.copyFileTo(newFilePath);
                }
            }
        }
    }
    
    
    void exportCSVButtonClicked() // This function creates a file, then uses an output stream to make the file into CSV, with a column for embedded original filename and another column for current filename. SFM will use the csv in google sheets.
    {
        juce::String CSVtext;
        
        for (auto& file : currentlyLoadedFiles)
        {
            auto currentFilename = file.getFileName();
            int indexOfFile = currentlyLoadedFiles.indexOf(file);
            
            auto& metadata = currentlyLoadedReaders.getReference(indexOfFile)->metadataValues;
            
            auto embeddedFilename = metadata.getValue(juce::WavAudioFormat::riffInfoMedium, "ERROR - didn't return value for riffInfoMedium");
            
            juce::String currentCSVData = (juce::String(currentFilename) + juce::String(",") + juce::String (embeddedFilename) + juce::String("\n"));
            
            CSVtext += currentCSVData;
        }
        
        
        DBG(CSVtext);
        auto currentTime = juce::Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S");
        
        juce::File desktop(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory));
        juce::File outputFile (desktop.getChildFile("WavHeaderEditor filenames " + currentTime + ".csv"));
        
        if (CSVtext.length() > 1)
        {
            outputFile.replaceWithText(CSVtext);
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Exported embedded filenames to: ", outputFile.getFullPathName ());
            outputFile.create();
        }
    }
    
    void originalFilenameButtonClicked()  // this functions implements replacing the metadata itself because it is unique in that it needs to get some info from each file before setting the metadata.
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
//        int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::WarningIcon, "", "Are you sure you want to embed the current filename?");
//        if (result != 1)
//            return;
        
        if (!editOnlySelectedEnabled)
        {
            for (const auto& file : currentlyLoadedFiles)
            {
                const auto& filename = file.getFileName();
                int indexOfFile = currentlyLoadedFiles.indexOf(file);
                
                auto& metadata = currentlyLoadedReaders.getReference(indexOfFile)->metadataValues;
                metadata.set(juce::WavAudioFormat::riffInfoMedium, filename);
                wavFormat.replaceMetadataInFile(file, metadata);
            }
        }
        else
        {
            auto selectedRows = loadedFilesListBox.getSelectedRows();
            for (auto i = 0; i < selectedRows.size(); ++i)
            {
                auto rowIndex = selectedRows[i];
                
                const auto& filename = currentlyLoadedFiles.getReference(rowIndex).getFileName().toStdString();
                auto& metadata = currentlyLoadedReaders.getReference(rowIndex)->metadataValues;
                metadata.set(juce::WavAudioFormat::riffInfoMedium, filename);
                wavFormat.replaceMetadataInFile(currentlyLoadedFiles.getReference(rowIndex), metadata);
            }
        }
        
        
        
        updateMetadataDisplay();
    }
    
    void currentPeakVolumeButtonClicked()  // this one needs to read the sample data, so it also implements metadata replacement itself
    {
        if (currentlyLoadedFiles.isEmpty())
            return;
        
        int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::WarningIcon, "", "Are you sure you want to embed the current peak volume?");
        if (result != 1)
            return;
        
        if (!editOnlySelectedEnabled)
        {
            for (auto i = 0; i < currentlyLoadedReaders.size(); ++i)
            {
                auto& file = currentlyLoadedFiles.getReference(i);
                auto& reader = currentlyLoadedReaders.getReference(i);
                
                // get the biggest sample in the wav file, it ranges from -1.0 to 1.0
                juce::int64 numSamples = reader.get()->lengthInSamples;
                
                float lowestLeft = 0.0f;
                float highestLeft = 0.0f;
                float lowestRight = 0.0f;
                float highestRight = 0.0f;
                
                // find the max levels...
                reader.get()->readMaxLevels(0, numSamples, lowestLeft, highestLeft, lowestRight, highestRight);
                
                float maxLevel = juce::jmax(lowestLeft * -1, highestLeft, lowestRight * -1, highestRight);
                
                // convert it to decibels
                juce::String maxLevelAsDecibels = juce::Decibels::toString(juce::Decibels::gainToDecibels(maxLevel));
                
                // put it in the metadata
                auto& metadata = reader->metadataValues;
                metadata.set(juce::WavAudioFormat::riffInfoGenre, (juce::String("Embedded peak level: ") + maxLevelAsDecibels));
                wavFormat.replaceMetadataInFile(file, metadata);
            }
        }
        else // same as above
        {
            auto selectedRows = loadedFilesListBox.getSelectedRows();
            for (auto i = 0; i < selectedRows.size(); ++i)
            {
                auto rowIndex = selectedRows[i];
                auto& file = currentlyLoadedFiles.getReference(rowIndex);
                auto& reader = currentlyLoadedReaders.getReference(rowIndex);
                
                juce::int64 numSamples = reader.get()->lengthInSamples;
                
                float lowestLeft = 0.0f;
                float highestLeft = 0.0f;
                float lowestRight = 0.0f;
                float highestRight = 0.0f;
                
                reader.get()->readMaxLevels(0, numSamples, lowestLeft, highestLeft, lowestRight, highestRight);
                
                float maxLevel = juce::jmax(lowestLeft * -1, highestLeft, lowestRight * -1, highestRight);
                
                juce::String maxLevelAsDecibels = juce::Decibels::toString(juce::Decibels::gainToDecibels(maxLevel));
                
                auto& metadata = reader->metadataValues;
                metadata.set(juce::WavAudioFormat::riffInfoGenre, (juce::String("Embedded peak level: ") + maxLevelAsDecibels));
                wavFormat.replaceMetadataInFile(file, metadata);
            }
        }
        updateMetadataDisplay();
    }

    // Metadata
    
    void setSpecificMetadata(std::string key, std::string value)
    {
        if (currentlyLoadedReaders.isEmpty())
        {
            printMessage("no files loaded!");
            return;
        }
        
        if (!editOnlySelectedEnabled)
        {
            for (int i = 0; i < currentlyLoadedFiles.size(); i++)
            {
                auto& metadata = currentlyLoadedReaders.getReference(i)->metadataValues;
                metadata.set(key, value);
                wavFormat.replaceMetadataInFile(currentlyLoadedFiles.getReference(i), metadata);
            }
        }
        else
        {
            auto selectedRows = loadedFilesListBox.getSelectedRows();
            for (auto i = 0; i < selectedRows.size(); ++i)
            {
                auto rowIndex = selectedRows[i];
                auto fileIndex = filteredFilesIndices[rowIndex];
                auto& metadata = currentlyLoadedReaders.getReference(fileIndex)->metadataValues;
                metadata.set(key, value);
                wavFormat.replaceMetadataInFile(currentlyLoadedFiles.getReference(fileIndex), metadata);
            }
        }
        updateMetadataDisplay();
    }
    
    void removeSpecificMetadata(std::string key)
    {
        if (currentlyLoadedReaders.isEmpty())
        {
            printMessage("no files loaded!");
            return;
        }
    
        if (!editOnlySelectedEnabled)
        {
            for (int i = 0; i < currentlyLoadedFiles.size(); i++)
            {
                auto& metadata = currentlyLoadedReaders.getReference(i)->metadataValues;
                metadata.remove(key);
                wavFormat.replaceMetadataInFile(currentlyLoadedFiles.getReference(i), metadata);
            }
        }
        else
        {
            auto selectedRows = loadedFilesListBox.getSelectedRows();
            for (auto i = 0; i < selectedRows.size(); ++i)
            {
                auto rowIndex = selectedRows[i];
                auto fileIndex = filteredFilesIndices[rowIndex];
                auto& metadata = currentlyLoadedReaders.getReference(fileIndex)->metadataValues;
                metadata.remove(key);
                wavFormat.replaceMetadataInFile(currentlyLoadedFiles.getReference(fileIndex), metadata);
                
            }
        }
        updateMetadataDisplay();
    }
    
    // Files
    
    void loadSamples(const juce::StringArray& files)
    {
        if (files.isEmpty()) { return; }
        
//        // uncomment these lines if you want loading new files to replace the currently loaded files.
//        currentlyLoadedReaders.clear();
//        currentlyLoadedFiles.clear();
//        filteredFilesIndices.clear();

        for (const auto& filePath : files)
        {
            juce::File currentFile(filePath);
            
            if (!currentlyLoadedFiles.contains(currentFile))
            {
                if (currentFile.getFileExtension() == ".wav")
                {
                    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(currentFile));
                    if (reader != nullptr)
                    {
                        if (
                            /*! (currentlyLoadedReaders.contains(reader)) &&*/  // Is there another way to check if the reader is already loaded? This line breaks it
                            !currentlyLoadedFiles.contains(currentFile))
                        {
                            currentlyLoadedReaders.add(std::move(reader));
                            currentlyLoadedFiles.add(currentFile);
                        }
                    }
                }
            }
        }
        systemMessagesDisplay.insertTextAtCaret(juce::String("Loaded ") + (juce::String)(currentlyLoadedFiles.size()) + juce::String(" files.") + juce::newLine);
        
        updateLoadedFilesListBox();
        updateMetadataDisplay();
    }

    void clearLoadedFiles()
    {
        currentlyLoadedReaders.clear();
        filteredFilesIndices.clear();
        currentlySelectedFileIndex = 0;
        updateLoadedFilesListBox();
        updateMetadataDisplay();
    }
    
    void clearSelectedFiles()
    {
        auto selectedRows = loadedFilesListBox.getSelectedRows();
        
        
        for (int i = 0; i < selectedRows.size(); i++)
        {
           // get index of filtered files
             auto rowIndex = selectedRows[i];
            
             auto filteredFilesRowIndex = filteredFilesIndices[rowIndex];
            
            currentlyLoadedFiles.remove(filteredFilesRowIndex);
            currentlyLoadedReaders.remove(filteredFilesRowIndex);
                         
        }
        
        
        updateLoadedFilesListBox();
        updateMetadataDisplay();
    }
    
    // Filters
    
    bool checkCustomFilterMatch(juce::StringPairArray& metadata) const
    {
        juce::String searchTerm = customFilterTextEditor.getText();

        for (const auto& value : metadata.getAllValues())
        {
            if (value.containsIgnoreCase(searchTerm))
                return true;
        }
        return false;
    }
    
    void updateFilteredFiles()
    {
        filteredFilesIndices.clear();

        for (const auto& file : currentlyLoadedFiles)
        {
            
            int indexOfFile = currentlyLoadedFiles.indexOf(file);
            
            juce::StringPairArray& metadataValues = currentlyLoadedReaders.getReference(indexOfFile)->metadataValues;
            
            bool isTrimmed = (metadataValues.getValue(juce::WavAudioFormat::riffInfoArtist, "NOT TRIMMED")) == "Trimmed";
            bool isFaded = (metadataValues.getValue(juce::WavAudioFormat::riffInfoCopyright, "NOT FADED")) == "Faded";
            bool isRxed = (metadataValues.getValue(juce::WavAudioFormat::riffInfoKeywords, "NOT RXED")) == "RXed";
            bool isFinalized = (metadataValues.getValue(juce::WavAudioFormat::riffInfoArchivalLocation, "NOT FINALIZED")) == "Finalized";
            bool isCustomFilterMatch = (checkCustomFilterMatch(metadataValues));

            if ((!trimmedFilterToggleEnabled || (trimmedFilterToggleEnabled && isTrimmed)) &&
                (!fadedFilterToggleEnabled || (fadedFilterToggleEnabled && isFaded)) &&
                (!rxedFilterToggleEnabled || (rxedFilterToggleEnabled && isRxed)) &&
                (!finalizedFilterToggleEnabled || (finalizedFilterToggleEnabled && isFinalized)) &&
                (!customFilterToggleEnabled || (customFilterToggleEnabled && isCustomFilterMatch)))
            {
                filteredFilesIndices.add(currentlyLoadedFiles.indexOf(file));
            }
        }
    }
    
    
    void customFilterTextEditorChanged()
    {
        if (customFilterToggleEnabled)
            updateLoadedFilesListBox();
    }
    
    // ListBox (files display)

    void updateLoadedFilesListBox()
    {
        updateFilteredFiles();
        
        auto newListBoxModel = std::make_unique<SampleListBoxModel>(*this, filteredFilesIndices);

        loadedFilesListBoxModel = std::move(newListBoxModel);
        loadedFilesListBox.setModel(loadedFilesListBoxModel.get());
        loadedFilesListBox.updateContent();
        loadedFilesListBox.selectRow(currentlySelectedFileIndex);
        updateMetadataDisplay();
    }
    
    class SampleListBoxModel : public juce::ListBoxModel
    {
        public:
            SampleListBoxModel(MainComponent& parent, const juce::Array<int>& filteredFileIndicesOfParent)
                : mainComponent(parent), indices(filteredFileIndicesOfParent)
            {
                
            }

            void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
            {
                if (rowIsSelected)
                    g.fillAll(juce::Colours::lightblue);

                g.setColour(juce::Colours::black);
                g.setFont(height * 0.7f);

                if (rowNumber < indices.size())
                {
                    int indexInMainComponentArrays = indices.getUnchecked(rowNumber);
                    auto& file = mainComponent.currentlyLoadedFiles.getReference(indexInMainComponentArrays);
                    
                    juce::String padding = (rowNumber < 9) ? "0" : "";

                    auto text = padding + juce::String(rowNumber + 1) + ": " + file.getParentDirectory().getFileName() + "/" + file.getFileNameWithoutExtension();
                    
                    g.drawText(text, 5, 0, width, height, juce::Justification::centredLeft, true);
        
                    auto& reader = mainComponent.currentlyLoadedReaders.getReference(indexInMainComponentArrays);
                    
                    if (reader != nullptr)
                    {
                        if ((reader->metadataValues.getValue(juce::WavAudioFormat::riffInfoArtist, "NOT TRIMMED")) == "Trimmed")
                        {
                            g.setColour(juce::Colours::red);
                            g.drawRect(width - height * 4, 2, height-6, height-6);
                        }
                        
                        if ((reader->metadataValues.getValue(juce::WavAudioFormat::riffInfoCopyright, "NOT FADED")) == "Faded")
                        {
                            g.setColour(juce::Colours::orange);
                            g.drawRect(width - (height * 3), 2, height-6, height-6);
                        }
                        
                        if ((reader->metadataValues.getValue(juce::WavAudioFormat::riffInfoKeywords, "NOT RXED")) == "RXed")
                        {
                            g.setColour(juce::Colours::blue);
                            g.drawRect(width - (height * 2), 2, height-6, height-6);
                        }
                        
                        if ((reader->metadataValues.getValue(juce::WavAudioFormat::riffInfoArchivalLocation, "NOT FINALIZED")) == "Finalized")
                        {
                            g.setColour(juce::Colours::limegreen);
                            g.drawRect(width - (height), 2, height-6, height-6);
                        }
                    }
                }
            }

            void listBoxItemClicked(int row, const juce::MouseEvent&) override
            {
                
            }

            void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override
            {
                const auto& file = mainComponent.currentlyLoadedFiles.getReference(indices.getUnchecked(row));
                file.revealToUser();
            }
    
            void selectedRowsChanged(int lastRowSelected) override
            {
                if (lastRowSelected == -1)
                {
                    mainComponent.currentlySelectedFileIndex = 0;
                }
                else
                {
                    if (!mainComponent.currentlyLoadedFiles.isEmpty())
                    {
                        mainComponent.currentlySelectedFileIndex = indices.getUnchecked(lastRowSelected);
                    }
                }
                mainComponent.updateMetadataDisplay();
            }
        
            int getNumRows() override  { return indices.size(); }

        private:
            MainComponent& mainComponent;
            const juce::Array<int>& indices;
    };

    // Display
    
    void updateMetadataDisplay()
    {
        metadataDisplay.clear();
        
        if (!currentlyLoadedFiles.isEmpty())
        {
            metadataDisplay.clear();
            
            if (!currentlyLoadedFiles.isEmpty())
                metadataDisplay.insertTextAtCaret(currentlyLoadedFiles.getReference(currentlySelectedFileIndex).getFullPathName().toStdString() + juce::newLine);
            
            auto metadata = currentlyLoadedReaders.getReference(currentlySelectedFileIndex)->metadataValues;
            
            for (auto key : metadata.getAllKeys())
            {
                metadataDisplay.insertTextAtCaret(key + ": " + metadata.getValue(key,"no such key found!") + juce::newLine);
            }
        }
        
    }

    void printMessage(juce::String message)
    {
        systemMessagesDisplay.insertTextAtCaret(message + juce::newLine);
    }

    // Drag and drop
    
    class DragAndDropSamplesTarget : public juce::FileDragAndDropTarget, public Component
        {
        public:
            DragAndDropSamplesTarget(MainComponent& parent) : mainComponent(parent){}
            
            void paint (juce::Graphics& g) override
            {
                g.fillAll (juce::Colours::aliceblue);
                g.drawFittedText("Drag and Drop Your Files Here", getLocalBounds(), juce::Justification::centred, 2);
                if (somethingIsBeingDraggedOver)
                    g.drawRect (getLocalBounds(), 2);
            }
                
            void filesDropped(const juce::StringArray& files, int x, int y) override
            {
                
                juce::StringArray filesToLoad;
                
                for (const auto& filePath : files)
                {
                    juce::File file(filePath);
                    if (file.isDirectory())
                    {
                        juce::Array<juce::File> childFiles;
                        file.findChildFiles(childFiles, juce::File::findFiles, false, "*.wav");  // The third argument is recursive flag. Set it to true if you want to search for files in subdirectories.
                        for (const auto& childFile : childFiles)
                        {
                            filesToLoad.add(childFile.getFullPathName());
                        }
                    }
                    else
                    {
                        filesToLoad.add(filePath);
                    }
                }
                
                if (!filesToLoad.isEmpty())
                    mainComponent.loadSamples(filesToLoad);
                
                somethingIsBeingDraggedOver = false;
                repaint();
            }
            
            void fileDragEnter(const juce::StringArray& files, int x, int y) override { somethingIsBeingDraggedOver = true; repaint(); }
            void fileDragExit(const juce::StringArray& files) override { somethingIsBeingDraggedOver = false; repaint(); }
            bool isInterestedInFileDrag(const juce::StringArray& files) override { return true; }
            void fileDragMove (const juce::StringArray& /*files*/, int /*x*/, int /*y*/) override {}
            bool somethingIsBeingDraggedOver = false;
            
        private:
            MainComponent& mainComponent;
        };
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::grey);
        
        g.setColour (juce::Colours::red);
        g.drawRect (trimmedToggleButton.getBounds().reduced(3), 1);
        g.drawRect (trimmedFilterToggleButton.getBounds().reduced(3), 1);
        g.setOpacity(0.5f);
        g.drawRect (doItButton.getBounds().expanded(1), 1);
        g.setColour (juce::Colours::orange);
        g.drawRect (fadedToggleButton.getBounds().reduced(3), 1);
        g.drawRect (fadedFilterToggleButton.getBounds().reduced(3), 1);
        g.setOpacity(0.5f);
        g.drawRect (doItButton.getBounds().expanded(2), 1);
        g.setColour (juce::Colours::blue);
        g.drawRect (rxedToggleButton.getBounds().reduced(3), 1);
        g.drawRect (rxedFilterToggleButton.getBounds().reduced(3), 1);
        g.setOpacity(0.5f);
        g.drawRect (doItButton.getBounds().expanded(3), 1);
        g.setColour (juce::Colours::limegreen);
        g.drawRect (finalizedToggleButton.getBounds().reduced(3), 1);
        g.drawRect (finalizedFilterToggleButton.getBounds().reduced(3), 1);
        g.setOpacity(0.5f);
        g.drawRect (doItButton.getBounds().expanded(4), 1);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(5);
        auto width = bounds.getWidth();

        auto topPanel = bounds.removeFromTop(120);
        auto leftPanel  = bounds.removeFromLeft(width / 2).reduced(5);
        auto rightPanel = bounds.reduced(5);

        int numberOfSections = 4;
        int sectionWidth = width / numberOfSections;
        
        int padding = 5;

        auto section1 = topPanel.removeFromLeft(sectionWidth);
        auto section2 = topPanel.removeFromLeft(sectionWidth);
        auto section3 = topPanel.removeFromLeft(sectionWidth);
        auto section4 = topPanel.removeFromLeft(sectionWidth);

        loadedFilesListBox.setBounds(leftPanel);
        metadataDisplay.setBounds(rightPanel);

        auto section1Bottom = section1.removeFromBottom(40);
        auto section1Top = section1;
        
        auto section1TopLeft = section1Top.removeFromLeft(sectionWidth/2);
        auto section1TopRight = section1Top;
        
        dragAndDropTarget.setBounds(section1TopLeft.reduced(2));
        loadButton.setBounds(section1TopRight.removeFromTop(40));
        clearButton.setBounds(section1TopRight);

        customFilterToggleButton.setBounds(section1Bottom.removeFromLeft(sectionWidth/2));
        customFilterTextEditor.setBounds(section1Bottom.reduced(padding));

        auto section2Top = section2.removeFromTop(40);
        auto section2TopA = section2Top;
        auto section2TopB = section2TopA.removeFromRight(section2Top.getWidth()/2);
        
        auto section2Bottom = section2.removeFromBottom(40);
        auto section2BottomA = section2Bottom.removeFromLeft(sectionWidth / 4);
        auto section2BottomB = section2Bottom.removeFromLeft(sectionWidth / 4);
        auto section2BottomC = section2Bottom.removeFromLeft(sectionWidth / 4);
        auto section2BottomD = section2Bottom;
        
        trimmedFilterToggleButton.setBounds(section2BottomA);
        fadedFilterToggleButton     .setBounds(section2BottomB);
        rxedFilterToggleButton    .setBounds(section2BottomC);
        finalizedFilterToggleButton  .setBounds(section2BottomD);

        exportButton.setBounds(section2TopA);
        doItButton.setBounds(section2TopB.reduced(4));
        
        editOnlySelectedButton.setBounds(section2);
        
        auto section3Bottom = section3.removeFromBottom(40);
        auto section3Middle = section3.removeFromBottom(40);
        auto section3Top = section3;
        
        auto section3a = section3Top.removeFromLeft(section3.getWidth()/2);
        auto section3b = section3Top;
        auto section3c = section3Middle.removeFromLeft(section3Bottom.getWidth()/2);
        auto section3d = section3Middle;

        trimmedToggleButton.setBounds(section3a);
        fadedToggleButton.setBounds(section3b);
        rxedToggleButton.setBounds(section3c);
        finalizedToggleButton.setBounds(section3d);
        
        auto section3BottomLeft = section3Bottom;
        auto section3BottomRight = section3BottomLeft.removeFromRight(section3Bottom.getWidth()/2);
        
        originalFilenameButton.setBounds(section3BottomLeft);
        exportCSVButton.setBounds(section3BottomRight);
        
//        currentPeakVolumeButton.setBounds(section3BottomLeft);
//        originalFilenameButton.setBounds(section3BottomRight);
        
        auto section4Bottom = section4.removeFromBottom(40);
        auto section4Middle = section4.removeFromBottom(40);

        auto section4a      = section4.removeFromLeft(section4.getWidth()/2);
        auto section4b      = section4;
        auto section4c      = section4Middle.removeFromLeft(section4Bottom.getWidth()/2);
        auto section4d      = section4Middle;

        auto section4e      = section4Bottom.removeFromLeft(section4Bottom.getWidth()/2);
        auto section4f      = section4Bottom;

        customTag1Button.setBounds(section4a);
        customTag1TextEditor.setBounds(section4b.reduced(padding));
        customTag2Button.setBounds(section4c);
        customTag2TextEditor.setBounds(section4d.reduced(padding));
        kitNameButton.setBounds(section4e);
        kitNameTextEditor.setBounds(section4f.reduced(padding));
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {    }
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override { bufferToFill.clearActiveBufferRegion(); }
    void releaseResources() override    {    }

private:
    std::unique_ptr<juce::LookAndFeel_V3> lookAndFeel;

    juce::Array<std::unique_ptr<juce::AudioFormatReader>> currentlyLoadedReaders;
    juce::Array<juce::File>                               currentlyLoadedFiles;
    juce::Array<int>                                      filteredFilesIndices;

    juce::AudioDeviceManager deviceManager;
    juce::AudioFormatManager formatManager;
    juce::WavAudioFormat wavFormat;

    juce::TextEditor metadataDisplay, systemMessagesDisplay, customTag1TextEditor, customTag2TextEditor, kitNameTextEditor, customFilterTextEditor;

    juce::TextButton loadButton {"Load Files"}, clearButton {"Clear Files"}, doItButton {"Do it!"};
    juce::TextButton customTag1Button {"Custom Tag 1"}, customTag2Button {"Custom Tag 2"};
    juce::TextButton originalFilenameButton { "Embed current Filename"};
    juce::TextButton kitNameButton { "Kit Name"};
    juce::TextButton exportButton {"Export"};
    juce::TextButton exportCSVButton {"Export CSV"};
    juce::TextButton currentPeakVolumeButton { "Embed current peak volume"};
    
    juce::ToggleButton editOnlySelectedButton{"Edit/Export Only Selected Files"};
    bool editOnlySelectedEnabled = true;
    juce::ToggleButton trimmedToggleButton {"Trimmed"}, fadedToggleButton {"Faded"}, rxedToggleButton {"RXed"}, finalizedToggleButton {"Finalized"};
    bool trimmedToggleEnabled {false}, fadedToggleEnabled {false}, rxedToggleEnabled {false}, finalizedToggleEnabled {false};
    juce::ToggleButton trimmedFilterToggleButton {"Trim"}, fadedFilterToggleButton {"Fade"}, rxedFilterToggleButton {"RX"}, finalizedFilterToggleButton {"Fin"};
    bool trimmedFilterToggleEnabled {false}, fadedFilterToggleEnabled {false}, rxedFilterToggleEnabled {false}, finalizedFilterToggleEnabled {false};
    juce::ToggleButton customFilterToggleButton {"Custom Filter"};
    bool customFilterToggleEnabled {false};
     
    std::unique_ptr<SampleListBoxModel> loadedFilesListBoxModel;
    juce::ListBox loadedFilesListBox;
    int currentlySelectedFileIndex = 0;
    
    DragAndDropSamplesTarget dragAndDropTarget;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
