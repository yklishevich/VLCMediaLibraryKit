/*****************************************************************************
 * MLSearchAggregate.m
 * MediaLibraryKit
 *****************************************************************************
 * Copyright (C) 2010-2017 VLC authors and VideoLAN
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

#import "MLSearchAggregate.h"

@implementation MLSearchAggregate

- (instancetype)initWithAlbums:(NSArray<MLAlbum *> *)albums artists:(NSArray<MLArtist *> *)artists genres:(NSArray<MLGenre *> *)genres mediaSearchAggregate:(MLMediaSearchAggregate *)mediaSearchAggregate playlists:(NSArray<MLPlaylist *> *)playlists
{
    self = [super init];
    if (self) {
        _albums = albums;
        _artists = artists;
        _genres = genres;
        _mediaSearchAggregate = mediaSearchAggregate;
        _playlists = playlists;
    }
    return self;
}

+ (instancetype)initWithAlbums:(NSArray<MLAlbum *> *)albums artists:(NSArray<MLArtist *> *)artists genres:(NSArray<MLGenre *> *)genres mediaSearchAggregate:(MLMediaSearchAggregate *)mediaSearchAggregate playlists:(NSArray<MLPlaylist *> *)playlists
{
    return [[MLSearchAggregate alloc] initWithAlbums:albums
                                             artists:artists
                                              genres:genres
                                mediaSearchAggregate:mediaSearchAggregate
                                           playlists:playlists];
}

@end
