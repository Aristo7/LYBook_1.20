/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#ifndef ASSETSCANFOLDERINFO_H
#define ASSETSCANFOLDERINFO_H

#include <QString>
#include <AzCore/base.h>

namespace AssetProcessor
{
    struct AssetFileInfo
    {
        AssetFileInfo() = default;
        AssetFileInfo(QString filePath, AZ::u64 modTime, bool isDirectory) : m_filePath(filePath), m_modTime(modTime), m_isDirectory(isDirectory) {}

        bool operator==(const AssetFileInfo& rhs) const
        {
            return m_filePath == rhs.m_filePath && m_modTime == rhs.m_modTime && m_isDirectory == rhs.m_isDirectory;
        }

        QString m_filePath;
        AZ::u64 m_modTime;
        bool m_isDirectory;
    };

    inline uint qHash(const AssetFileInfo& item)
    {
        return qHash(item.m_filePath);
    }

    /** This Class contains information about the folders to be scanned
     * */
    class ScanFolderInfo
    {
    public:
        ScanFolderInfo(
            QString path,
            QString displayName,
            QString portableKey,
            QString prefix,
            bool isRoot,
            bool recurseSubFolders,
            AZStd::vector<AssetBuilderSDK::PlatformInfo> platforms = AZStd::vector<AssetBuilderSDK::PlatformInfo>{},
            int order = 0,
            AZ::s64 scanFolderID = 0,
            bool canSaveNewAssets = false)
            : m_scanPath(path)
            , m_displayName(displayName)
            , m_portableKey (portableKey)
            , m_outputPrefix(prefix)
            , m_isRoot(isRoot)
            , m_recurseSubFolders(recurseSubFolders)
            , m_order(order)
            , m_scanFolderID(scanFolderID)
            , m_platforms(platforms)
            , m_canSaveNewAssets(canSaveNewAssets)
        {
            // note that m_scanFolderID is 0 unless its filled in from the DB.
        }

        ScanFolderInfo() = default;
        ScanFolderInfo(const ScanFolderInfo& other) = default;

        QString ScanPath() const
        {
            return m_scanPath;
        }

        QString GetDisplayName() const
        {
            return m_displayName;
        }

        QString GetOutputPrefix() const
        {
            return m_outputPrefix;
        }

        bool IsRoot() const
        {
            return m_isRoot;
        }

        bool RecurseSubFolders() const
        {
            return m_recurseSubFolders;
        }

        bool CanSaveNewAssets() const
        {
            return m_canSaveNewAssets;
        }

        int GetOrder() const
        {
            return m_order;
        }

        AZ::s64 ScanFolderID() const
        {
            return m_scanFolderID;
        }

        QString GetPortableKey() const
        {
            return m_portableKey;
        }

        const AZStd::vector<AssetBuilderSDK::PlatformInfo>& GetPlatforms() const
        {
            return m_platforms;
        }

        void SetScanFolderID(AZ::s64 scanFolderID)
        {
            m_scanFolderID = scanFolderID;
        }

    private:
        QString m_scanPath; // the local path to scan ("C:\\whatever")
        QString m_displayName; // the display name to show in GUIs that show it.
        QString m_outputPrefix; // the output prefix to target results into (eg, put things in a certain subfolder of @assets@ rather than the relative to assets itself)
        QString m_portableKey; // a key that remains the same even if the asset database is moved from computer to computer.
        bool m_isRoot = false; // is it 'the' root folder?
        bool m_recurseSubFolders = true;
        bool m_canSaveNewAssets = false; // Tracks if it is safe to save new assets in this folder.
        int m_order = 0;
        AZ::s64 m_scanFolderID = 0; // this is filled in by the database - don't modify it.
        AZStd::vector<AssetBuilderSDK::PlatformInfo> m_platforms; // This contains the list of platforms that are enabled for the particular scanfolder
    };
} // end namespace AssetProcessor

#endif //ASSETSCANFOLDERINFO_H
