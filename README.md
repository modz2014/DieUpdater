# DieMain Updater

This repository contains the source code for the DieMain updater application. The purpose of this application is to facilitate updates for another software component. Below is a concise overview of the main features and functionality:

## Overview

1. **User Interface:**
   - The application creates a simple window with a title bar and controls.
   - Users can toggle advanced options using a checkbox.
   - Supports dynamic UI updates based on system theme changes (light/dark mode).

2. **Download and Update:**
   - Initiates a separate thread to check for updates.
   - Downloads a file from a specified URL using WinINet.
   - Displays progress using a progress bar.

3. **RichEdit Control:**
   - Dynamically creates a RichEdit control based on user interaction.
   - Adjusts text and background colors based on the system theme.

4. **Theme Management:**
   - Dynamically updates the application's theme based on system changes.
   - Manages text and background colors for different themes (light/dark).

5. **ZIP Extraction:**
   - Downloads a ZIP file and extracts its contents using PowerShell.
   - Provides feedback in a RichEdit control during the download and extraction process.

## Building and Running the Application

To build and run the DieMain updater, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-username/DieMainUpdater.git
   cd DieMainUpdater
 **Run the Application:**
    Execute the built executable to launch the DieMain updater.

# Usage

- The application provides a checkbox for toggling advanced options.
- Check for updates and initiate the download process.

# Dependencies

- The application uses the WinINet library for network operations.
- GDI+ is utilized for graphics-related tasks.
- The RichEdit control is employed for displaying dynamic text.

- ## Example Update JSON

Here's an example JSON code for update information:

```json
{
  "updates": [
    {
      "latestVersion": "2.0.0",
      "releaseDate": "2023-12-01",
      "downloadUrl": "Url for the file to be downloaded",
      "fileName": "ILSpy_binaries_8.2.0.7535-x64.zip",
      "releaseNotes": "Release notes for version 2.0.0..."
    }
  ]
}


# Notes

- The source includes comments for better understanding.
- Ensure proper error handling and edge case management for production use.

Feel free to explore the source code and adapt it to your specific requirements. If you encounter any issues or have suggestions, please contribute to the repository.
