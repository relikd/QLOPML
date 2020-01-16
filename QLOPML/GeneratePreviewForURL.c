#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>
#include "opml-lib.h"

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview);

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
	// qlmanage -r && qlmanage -p test.opml -o tmp/ && edit tmp/test.opml.qlpreview/Preview.html
	CFBundleRef bundle = QLPreviewRequestGetGeneratorBundle(preview);
	CFDataRef data = renderOPML(url, bundle);
	if (data) {
		QLPreviewRequestSetDataRepresentation(preview, data, kUTTypeHTML, NULL);
		CFRelease(data);
	}
    return noErr;
}

void CancelPreviewGeneration(void *thisInterface, QLPreviewRequestRef preview)
{
    // Implement only if supported
}
