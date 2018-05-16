/*****************************************************************************
 * VLCMLFile.h
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

typedef NS_ENUM(NSInteger, VLCMLFileType) {
    /// Unknown type, so far
    Unknown,
    /// The main file of a media.
    Main,
    /// A part of a media (for instance, the first half of a movie)
    Part,
    /// External soundtrack
    Soundtrack,
    /// External subtitles
    Subtitles
};

@interface VLCMLFile : NSObject <VLCMLObject>

@property (nonatomic, copy) NSString *mrl;

- (instancetype)init NS_UNAVAILABLE;

- (VLCMLIdentifier)identifier;
- (NSString *)mrl;
- (VLCMLFileType)type;
- (uint)lastModificationDate;
- (uint)size;
- (BOOL)isExternal;

@end