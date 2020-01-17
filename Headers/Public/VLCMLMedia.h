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

NS_ASSUME_NONNULL_BEGIN

@class VLCMLAlbum, VLCMLAlbumTrack, VLCMLShowEpisode, VLCMLMetadata, VLCMLLabel, VLCMLShowEpisode, VLCMLMovie, VLCMLFile, VLCMLAudioTrack, VLCMLVideoTrack, VLCMLSubtitleTrack;

typedef NS_ENUM(NSInteger, VLCMLFileType);
typedef NS_ENUM(NSUInteger, VLCMLThumbnailSizeType);
typedef NS_ENUM(NSUInteger, VLCMLThumbnailStatus);

typedef NS_ENUM(UInt8, VLCMLMediaType) {
    /**
     * Unknown media type. Type is used to avoid 0 being a valid value
     * Media that are discovered by the medialibrary will not be
     * added to the collection when their type can't be determined
     */
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

@property (nonatomic, assign) float progress;
@property (nonatomic, assign) BOOL isNew;
@property (nonatomic, assign) SInt64 audioTrackIndex;
@property (nonatomic, assign) SInt64 subtitleTrackIndex;
@property (nonatomic, assign) SInt64 chapterIndex;
@property (nonatomic, assign) SInt64 titleIndex;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, strong, nullable) VLCMLAlbumTrack *albumTrack;
@property (nonatomic, strong, nullable) VLCMLShowEpisode *showEpisode;
@property (nonatomic, strong, nullable) VLCMLMovie *movie;

@property (nonatomic, copy) NSArray<VLCMLFile *> *files;
@property (nonatomic, copy, nullable) NSArray<VLCMLLabel *> *labels;
@property (nonatomic, copy, nullable) NSArray<VLCMLAudioTrack *> *audioTracks;
@property (nonatomic, copy, nullable) NSArray<VLCMLVideoTrack *> *videoTracks;
@property (nonatomic, copy, nullable) NSArray<VLCMLSubtitleTrack *> *subtitleTracks;

- (instancetype)init NS_UNAVAILABLE;

#pragma mark - Getters/Setters

- (VLCMLMediaType)type;
- (VLCMLMediaSubtype)subtype;

- (BOOL)updateTitle:(NSString *)title;
- (SInt64)duration;
- (int)playCount;
- (BOOL)increasePlayCount;
- (BOOL)setPlayCount:(UInt32)playCount;

/**
 * \return The main file's filename
 */
- (NSString *)fileName;

- (nullable VLCMLFile *)mainFile;
- (nullable VLCMLFile *)addExternalMrl:(NSURL *)mrl fileType:(VLCMLFileType)type;

- (BOOL)isFavorite;
- (BOOL)setFavorite:(BOOL)favorite;

- (BOOL)addLabel:(VLCMLLabel *)label;
- (BOOL)removeLabel:(VLCMLLabel *)label;

/**
 * \brief thumbnail Returns the mrl of a thumbnail of the given size for this media
 * \param sizeType The targeted thumbnail size
 * \return An mrl, representing the absolute path to the media thumbnail
 *         or nil, if the thumbnail generation failed
 *
 * \note By default this returns the mrl for VLCMLThumbnailSizeTypeThumbnail
 * \sa{thumbnailStatus}
 */
- (nullable NSURL *)thumbnail;
- (nullable NSURL *)thumbnailOfType:(VLCMLThumbnailSizeType)type;

/**
 * @brief thumbnailStatus Returns this media thumbnail status
 * @param type The targeted thumbnail size
 *
 * This will return Missing if no thumbnail generation has been requested
 * for this media, or Success/Failure/Crash, depending on the generation
 * results.
 */
- (VLCMLThumbnailStatus)thumbnailStatus;
- (VLCMLThumbnailStatus)thumbnailStatusOfType:(VLCMLThumbnailSizeType)type;

/**
 * \brief setThumbnailWithMRL Sets a thumbnail for the current media
 * \param mrl A mrl pointing the the thumbnail file.
 * \param type The targeted thumbnail size type
 * \return true in case the thumbnail was successfully stored to database
 *         false otherwise
 * This is intended to be used by applications that have their own way
 * of computing thumbnails.
 */
- (BOOL)setThumbnailWithMRL:(NSURL *)mrl ofType:(VLCMLThumbnailSizeType)type;

/**
 * \brief requestThumbnail Queues a thumbnail generation request for
 * this media, to be run asynchronously.
 * Upon completion (successful or not) IMediaLibraryCb::onMediaThumbnailReady
 * will be called.
 * In case a thumbnail was already generated for the media, a new thumbnail
 * will be generated, and the previous one will be overriden.
 * In case a previous thumbnailing attempt failed, false will be returned
 * and no new generation will occur.
 * If you want to force a new generation, you need to
 * call \sa{IMediaLibrary::enableFailedThumbnailRegeneration} beforehand.
 * \param sizeType The size type of the thumbnail to generate
 * \param desiredWidth The desired thumbnail width
 * \param desiredHeight The desired thumbnail height
 * \param position The position at which to generate the thumbnail, in [0;1] range
 *
 * The generated thumbnail will try to oblige by the requested size, while
 * respecting the source aspect ratio. If the aspect ratios differ, the
 * source image will be cropped.
 * If one of the dimension is 0, the other one will be deduced from the
 * source aspect ratio. If both are 0, the source dimensions will be used.
 *
 * This function is thread-safe
 */
- (BOOL)requestThumbnailOfType:(VLCMLThumbnailSizeType)type
                  desiredWidth:(NSUInteger)width
                 desiredHeight:(NSUInteger)height
                    atPosition:(float)position;

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
NS_ASSUME_NONNULL_END
