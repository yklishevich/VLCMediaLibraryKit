/*****************************************************************************
 * VLCMediaLibrary.m
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

#import "VLCMediaLibrary.h"
#import "VLCMLAlbumTrack.h"
#import "VLCMLAlbum+Init.h"
#import "VLCMLArtist+Init.h"
#import "VLCMLFolder+Init.h"
#import "VLCMLGenre+Init.h"
#import "VLCMLHistoryEntry+Init.h"
#import "VLCMLLabel+Init.h"
#import "VLCMLMedia+Init.h"
#import "VLCMLMovie+Init.h"
#import "VLCMLPlaylist+Init.h"
#import "VLCMLShow+Init.h"
#import "VLCMLUtils.h"
#import "VLCMLMediaSearchAggregate.h"
#import "VLCMLSearchAggregate.h"

#import "MediaLibraryCb.h"
#import "DeviceListerCb.h"
#import "VLCMLDeviceLister.h"

@interface VLCMediaLibrary ()
{
    BOOL _isInitialized;

    medialibrary::MediaLibraryCb *_mlCb;
    medialibrary::DeviceListerCb *_deviceListerCb;

    medialibrary::IMediaLibrary *_ml;
    medialibrary::DeviceListerPtr _deviceLister;
}
@end

@implementation VLCMediaLibrary

#pragma mark - Shared methods

+ (instancetype)sharedMediaLibrary
{
    static VLCMediaLibrary *sharedMediaLibrary = nil;
    static dispatch_once_t onceToken;

    dispatch_once(&onceToken, ^{
        sharedMediaLibrary = [[VLCMediaLibrary alloc] init];
    });

    return sharedMediaLibrary;
}

#pragma mark - Initializer

- (instancetype)init
{
    self = [super init];
    if (self) {
        _isInitialized = NO;
        _ml = NewMediaLibrary();
        _mlCb = new medialibrary::MediaLibraryCb(_delegate);
        _deviceListerCb = new medialibrary::DeviceListerCb(_deviceListerDelegate);
    }
    return self;
}

- (void)dealloc
{
    if (_mlCb) {
        delete _mlCb;
    }
    if (_deviceListerCb) {
        delete _deviceListerCb;
    }
    if (_ml) {
        delete _ml;
    }
}

- (void)setDelegate:(id<VLCMediaLibraryDelegate>)delegate
{
    if (_delegate != delegate) {
        _delegate = delegate;
        _mlCb->setDelegate(_delegate);
    }
}

- (void)setDeviceListerDelegate:(id<VLCMLDeviceListerDelegate>)deviceListerDelegate
{
    if (_deviceListerDelegate != deviceListerDelegate) {
        _deviceListerDelegate = deviceListerDelegate;
        _deviceListerCb->setDelegate(_deviceListerDelegate);
    }
}

- (BOOL)start
{
    BOOL success = _ml->start();

    return success;
}

- (VLCMLInitializeResult)setupMediaLibraryWithDb:(NSString *)dbPath thumbnailPath:(NSString *)thumbnailPath
{
    [self setDeviceLister:(_deviceLister)];
    VLCMLInitializeResult result = (VLCMLInitializeResult)_ml->initialize([dbPath UTF8String], [thumbnailPath UTF8String], _mlCb);

    if (result == VLCMLInitializeResultSuccess) {
        _isInitialized = YES;
        _dbPath = dbPath;
        _thumbnailPath = thumbnailPath;
    }
    return result;
}

- (void)setVerbosity:(VLCMLLogLevel)level
{
    _ml->setVerbosity((medialibrary::LogLevel)level);
}

#pragma mark -
#pragma mark Medialibrary main methods

#pragma mark - Label

- (VLCMLLabel *)createLabelWithName:(NSString *)name
{
    return [[VLCMLLabel alloc] initWithLabelPtr:_ml->createLabel([name UTF8String])];
}

- (BOOL)deleteLabel:(VLCMLLabel *)label
{
    return _ml->deleteLabel([label labelPtr]);
}

#pragma mark - Media

- (VLCMLMedia *)mediaWithIdentifier:(int64_t)identifier
{
    return [[VLCMLMedia alloc] initWithMediaPtr:_ml->media(identifier)];
}

- (VLCMLMedia *)mediaWithMrl:(NSString *)mrl
{
    return [[VLCMLMedia alloc] initWithMediaPtr:_ml->media([mrl UTF8String])];
}

- (VLCMLMedia *)addMediaWithMrl:(NSString *)mrl
{
    return [[VLCMLMedia alloc] initWithMediaPtr:_ml->addMedia([mrl UTF8String])];
}

- (NSArray<VLCMLMedia *> *)audioFilesWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
    return [VLCMLUtils arrayFromMediaPtrVector:_ml->audioFiles((medialibrary::SortingCriteria)criteria, desc)];
}

- (NSArray<VLCMLMedia *> *)videoFilesWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
    return [VLCMLUtils arrayFromMediaPtrVector:_ml->videoFiles((medialibrary::SortingCriteria)criteria, desc)];
}

#pragma mark - Album

- (VLCMLAlbum *)albumWithIdentifier:(int64_t)identifier
{
    return [[VLCMLAlbum alloc] initWithAlbumPtr:_ml->album(identifier)];
}

- (NSArray<VLCMLAlbum *> *)albumsWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
    return [VLCMLUtils arrayFromAlbumPtrVector:_ml->albums((medialibrary::SortingCriteria)criteria, desc)];
}

#pragma mark - Show

- (VLCMLShow *)showWithName:(NSString *)name
{
    return [[VLCMLShow alloc] initWithShowPtr:_ml->show([name UTF8String])];
}

#pragma mark - Movie

- (VLCMLMovie *)movieWithName:(NSString *)name
{
    return [[VLCMLMovie alloc] initWithMoviePtr:_ml->movie([name UTF8String])];
}

#pragma mark - Artist

- (VLCMLArtist *)artistWithIdentifier:(int64_t)identifier
{
    return [[VLCMLArtist alloc] initWithArtistPtr:_ml->artist(identifier)];
}

- (NSArray<VLCMLArtist *> *)artistsWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
    return [VLCMLUtils arrayFromArtistPtrVector:_ml->artists((medialibrary::SortingCriteria)criteria, desc)];
}

#pragma mark - Genre

- (NSArray<VLCMLGenre *> *)genresWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
    return [VLCMLUtils arrayFromGenrePtrVector:_ml->genres((medialibrary::SortingCriteria)criteria, desc)];
}

- (VLCMLGenre *)genreWithIdentifier:(int64_t)identifier
{
    return [[VLCMLGenre alloc] initWithGenrePtr:_ml->genre(identifier)];
}

#pragma mark - Playlist

- (VLCMLPlaylist *)createPlaylistWithName:(NSString *)name
{
    return [[VLCMLPlaylist alloc] initWithPlaylistPtr:_ml->createPlaylist([name UTF8String])];
}

- (NSArray<VLCMLPlaylist *> *)playlistsWithSortingCriteria:(VLCMLSortingCriteria)criteria desc:(BOOL)desc
{
   return [VLCMLUtils arrayFromPlaylistPtrVector:_ml->playlists((medialibrary::SortingCriteria)criteria, desc)];
}

- (VLCMLPlaylist *)playlistWithIdentifier:(int64_t)identifier
{
    return [[VLCMLPlaylist alloc] initWithPlaylistPtr:_ml->playlist(identifier)];
}

- (BOOL)deletePlaylistWithIdentifier:(int64_t)identifier
{
    return _ml->deletePlaylist(identifier);
}

#pragma mark - History

- (BOOL)addMediaToStreamHistory:(VLCMLMedia *)media
{
    return _ml->addToStreamHistory([media mediaPtr]);
}

- (NSArray<VLCMLHistoryEntry *> *)lastStreamsPlayed
{
    auto history = _ml->lastStreamsPlayed();
    NSMutableArray *result = [NSMutableArray array];

    for (const auto &historyEntry : history) {
        [result addObject:[[VLCMLHistoryEntry alloc] initWithHistoryPtr:historyEntry]];
    }
    return result;
}

- (NSArray<VLCMLMedia *> *)lastMediaPlayed
{
    return [VLCMLUtils arrayFromMediaPtrVector:_ml->lastMediaPlayed()];
}

- (BOOL)clearHistory
{
    return _ml->clearHistory();
}

#pragma mark - Search

- (VLCMLMediaSearchAggregate *)_convertMediaSearchAggregate:(medialibrary::MediaSearchAggregate)searchResult
{
    return [VLCMLMediaSearchAggregate initWithEpisodes:[VLCMLUtils arrayFromMediaPtrVector:searchResult.episodes]
                                             movies:[VLCMLUtils arrayFromMediaPtrVector:searchResult.movies]
                                             others:[VLCMLUtils arrayFromMediaPtrVector:searchResult.others]
                                             tracks:[VLCMLUtils arrayFromMediaPtrVector:searchResult.tracks]];
}

- (VLCMLMediaSearchAggregate *)searchMedia:(NSString *)pattern
{
    return [self _convertMediaSearchAggregate:_ml->searchMedia([pattern UTF8String])];
}

- (NSArray<VLCMLPlaylist *> *)searchPlaylistsByName:(NSString *)name
{
    return [VLCMLUtils arrayFromPlaylistPtrVector:_ml->searchPlaylists([name UTF8String])];
}

- (NSArray<VLCMLAlbum *> *)searchAlbumsByPattern:(NSString *)pattern
{
    return [VLCMLUtils arrayFromAlbumPtrVector:_ml->searchAlbums([pattern UTF8String])];
}

- (NSArray<VLCMLGenre *> *)searchGenreByName:(NSString *)name
{
    return [VLCMLUtils arrayFromGenrePtrVector:_ml->searchGenre([name UTF8String])];
}

- (NSArray<VLCMLArtist *> *)searchArtistsByName:(NSString *)name
{
    return [VLCMLUtils arrayFromArtistPtrVector:_ml->searchArtists([name UTF8String])];
}

- (VLCMLSearchAggregate *)search:(NSString *)pattern
{
    medialibrary::SearchAggregate searchResult = _ml->search([pattern UTF8String]);

    return [VLCMLSearchAggregate initWithAlbums:[VLCMLUtils arrayFromAlbumPtrVector:searchResult.albums]
                                     artists:[VLCMLUtils arrayFromArtistPtrVector:searchResult.artists]
                                      genres:[VLCMLUtils arrayFromGenrePtrVector:searchResult.genres]
                        mediaSearchAggregate:[self _convertMediaSearchAggregate:searchResult.media]
                                   playlists:[VLCMLUtils arrayFromPlaylistPtrVector:searchResult.playlists]];
}

#pragma mark - Discover

- (void)discoverOnEntryPoint:(NSString *)path
{
    _ml->discover([path UTF8String]);
}

- (void)enableDiscoverNetwork:(BOOL)enable
{
    _ml->setDiscoverNetworkEnabled(enable);
}

- (NSArray<VLCMLFolder *> *)entryPoints
{
    auto entryPoints = _ml->entryPoints();
    NSMutableArray *result = [NSMutableArray array];

    for (const auto &entryPoint : entryPoints) {
        [result addObject:[[VLCMLFolder alloc] initWithFolderPtr:entryPoint]];
    }
    return result;
}

- (void)removeEntryPointWithPath:(NSString *)path
{
    _ml->removeEntryPoint([path UTF8String]);
}

#pragma mark - Folder

- (void)banFolderWithPath:(NSString *)path
{
    _ml->banFolder([path UTF8String]);
}

- (void)unbanFolderWithEntryPoint:(NSString *)entryPoint
{
    _ml->unbanFolder([entryPoint UTF8String]);
}

#pragma mark - Thumbnail

- (NSString *)thumbnailPath
{
    if (!_thumbnailPath) {
        _thumbnailPath = [[NSString alloc] initWithUTF8String:_ml->thumbnailPath().c_str()];
    }
    return _thumbnailPath;
}

#pragma mark - Logger

#pragma mark - Background Operation

- (void)pauseBackgroundOperations
{
    _ml->pauseBackgroundOperations();
}

- (void)resumeBackgroundOperations
{
    _ml->resumeBackgroundOperations();
}

#pragma mark - Reload

- (void)reload
{
    _ml->reload();
}

- (void)reloadEntryPoint:(NSString *)entryPoint
{
    _ml->reload([entryPoint UTF8String]);
}

#pragma mark - Parser

- (void)forceParserRetry
{
    _ml->forceParserRetry();
}

#pragma mark - DeviceLister

- (void)setDeviceLister:(medialibrary::DeviceListerPtr)lister
{
    _deviceLister = std::make_shared<medialibrary::fs::VLCMLDeviceLister>();
    _ml->setDeviceLister(lister);
}

#pragma mark -

@end
