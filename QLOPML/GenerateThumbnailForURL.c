#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#include "opml-lib.h"

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize);
void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail);

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
	// test with:
	// rm -rf ~/Library/QuickLook/QLOPML.qlgenerator && qlmanage -r && rsync -a ~/Library/Developer/Xcode/DerivedData/QLOPML-*/Build/Products/Debug/QLOPML.qlgenerator ~/Library/QuickLook/ && qlmanage -t sample.opml -s 512 -i
	CFBundleRef bundle = QLThumbnailRequestGetGeneratorBundle(thumbnail);
	CFDataRef data = generateHTML(url, bundle, true);
	if (data) {
		QLThumbnailRequestSetThumbnailWithDataRepresentation(thumbnail, data, kUTTypeHTML, NULL, NULL);
		CFRelease(data);
	}
    return noErr;
	
//	CGSize thumbSize = CGSizeMake(maxSize.width * (600/800.0), maxSize.height);
//	// Draw the webview in the correct context
//	CGContextRef context = QLThumbnailRequestCreateContext(thumbnail, thumbSize, false, NULL);
//
//	if (context) {
//		CFBundleRef bundle = QLThumbnailRequestGetGeneratorBundle(thumbnail);
//		renderThumbnail(url, bundle, context, maxSize);
//		QLThumbnailRequestFlushContext(thumbnail, context);
//		CFRelease(context);
//	}
//	return noErr;
}

void CancelThumbnailGeneration(void *thisInterface, QLThumbnailRequestRef thumbnail)
{
    // Implement only if supported
}
