/*****************************************************************************
 * VLCMLShowEpisode.h
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

@class VLCMLShow, VLCMLMedia;

@interface VLCMLShowEpisode : NSObject <VLCMLObject>

@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *shortSummary;
@property (nonatomic, copy) NSURL *artworkMrl;
@property (nonatomic, copy) NSString *tvdbId;
@property (nonatomic, strong) VLCMLShow *show;
@property (nonatomic, copy) NSArray<VLCMLMedia *> *files;

- (instancetype)init NS_UNAVAILABLE;

- (VLCMLIdentifier)identifier;
- (NSURL *)artworkMrl;
- (uint)episodeNumber;
- (NSString *)name;
- (uint)seasonNumber;
- (NSString *)shortSummary;
- (NSString *)tvdbId;
- (VLCMLShow *)show;
- (NSArray<VLCMLMedia *> *)files;

@end

