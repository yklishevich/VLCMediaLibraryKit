/*****************************************************************************
 * VLCMLMedia.h
 * VLCMediaLibraryKit
 *****************************************************************************
 * Copyright (C) 2010-2018 VLC authors and VideoLAN
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#import "VLCMLObject.h"

@class VLCMLAlbum, VLCMLAlbumTrack, VLCMLShowEpisode, VLCMLMetadata, VLCMLLabel, VLCMLShowEpisode, VLCMLMovie, VLCMLFile, VLCMLAudioTrack, VLCMLVideoTrack, VLCMLSubtitleTrack;

typedef NS_ENUM(NSInteger, VLCMLFileType);

typedef NS_ENUM(UInt8, VLCMLMediaType) {
    VLCMLMediaTypeUnknown,
    VLCMLMediaTypeVideo,
    VLCMLMediaTypeAudio
};

typedef NS_ENUM(UInt8, VLCMLMediaSubtype) {
    VLCMLMediaSubtypeUnknown,
    VLCMLMediaSubtypeShowEpisode,
    VLCMLMediaSubtypeMovie,
    VLCMLMediaSubtypeAlbumTrack
};

typedef NS_ENUM(UInt32, VLCMLMetadataType) {
    VLCMLMetadataTypeRating = 1,

    // Playback
    VLCMLMetadataTypeProgress = 50,
    VLCMLMetadataTypeSpeed,
    VLCMLMetadataTypeTitle,
    VLCMLMetadataTypeChapter,
    VLCMLMetadataTypeProgram,
    VLCMLMetadataTypeSeen,

    // Video:
    VLCMLMetadataTypeVideoTrack = 100,
    VLCMLMetadataTypeAspectRatio,
    VLCMLMetadataTypeZoom,
    VLCMLMetadataTypeCrop,
    VLCMLMetadataTypeDeinterlace,
    VLCMLMetadataTypeVideoFilter,

    // Audio
    VLCMLMetadataTypeAudioTrack = 150,
    VLCMLMetadataTypeGain,
    VLCMLMetadataTypeAudioDelay,

    // Spu
    VLCMLMetadataTypeSubtitleTrack = 200,
    VLCMLMetadataTypeSubtitleDelay,

    // Various
    VLCMLMetadataTypeApplicationSpecific = 250,
};

@interface VLCMLMedia : NSObject <VLCMLObject>

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSURL *thumbnail;
@property (nonatomic, strong) VLCMLAlbumTrack *albumTrack;
@property (nonatomic, strong) VLCMLShowEpisode *showEpisode;
@property (nonatomic, strong) VLCMLMovie *movie;
@property (nonatomic, copy) NSArray<VLCMLFile *> *files;
@property (nonatomic, copy) NSArray<VLCMLLabel *> *labels;
@property (nonatomic, copy) NSArray<VLCMLAudioTrack *> *audioTracks;
@property (nonatomic, copy) NSArray<VLCMLVideoTrack *> *videoTracks;
@property (nonatomic, copy) NSArray<VLCMLSubtitleTrack *> *subtitleTracks;

- (instancetype)init NS_UNAVAILABLE;

#pragma mark - Getters/Setters

- (VLCMLMediaType)type;
- (VLCMLMediaSubtype)subtype;

- (BOOL)updateTitle:(NSString *)title;
- (SInt64)duration;
- (int)playCount;
- (BOOL)increasePlayCount;
- (BOOL)setPlayCount:(UInt32)playCount;

- (VLCMLFile *)mainFile;
- (VLCMLFile *)addExternalMrl:(NSURL *)mrl fileType:(VLCMLFileType)type;

- (BOOL)isFavorite;
- (BOOL)setFavorite:(BOOL)favorite;

- (BOOL)addLabel:(VLCMLLabel *)label;
- (BOOL)removeLabel:(VLCMLLabel *)label;

- (BOOL)isThumbnailGenerated;

- (NSDate *)insertionDate;
- (NSDate *)releaseDate;

#pragma mark - Metadata

/**
 * @brief metadataOfType Returns the specified metadata contained by this media
 * @param type The metadata type VLCMLMetadataType
 * @return A VLCMLMetadata object with the metadata
 */
- (VLCMLMetadata *)metadataOfType:(VLCMLMetadataType)type;

/**
 * @brief setMetadataOfType Set the specified metadata contained by this media
 * @param type The metadata type VLCMLMetadataType
 * @param value A value for the said metadata
 * @return A Bool depending on success
 *
 * This function sets the metadata described by <type> to the given value
 * The C++ medialibrary translate and store the given data as a string
 * This means that the application is in charge of knowing the meaning
 * \of each value for the given type.
 */
- (BOOL)setMetadataOfType:(VLCMLMetadataType)type stringValue:(NSString *)value;
- (BOOL)setMetadataOfType:(VLCMLMetadataType)type intValue:(SInt64)value;

@end

