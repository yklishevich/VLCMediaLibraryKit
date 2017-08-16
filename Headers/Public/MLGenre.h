/*****************************************************************************
 * MLGenre.h
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

typedef NS_ENUM (NSUInteger, MLSortingCriteria);

@class MLArtist, MLMedia, MLAlbum;

@interface MLGenre : NSObject

@property (nonatomic, copy) NSString *name;

- (instancetype)init NS_UNAVAILABLE;

- (int64_t)identifier;
- (NSString *)name;
- (uint32_t)numberOfTracks;

- (NSArray<MLArtist *> *)artistWithSortingCriteria:(MLSortingCriteria)criteria desc:(BOOL)desc;
- (NSArray<MLMedia *> *)tracksWithSortingCriteria:(MLSortingCriteria)criteria desc:(BOOL)desc;
- (NSArray<MLAlbum *> *)albumsWithSortingCriteria:(MLSortingCriteria)criteria desc:(BOOL)desc;

@end