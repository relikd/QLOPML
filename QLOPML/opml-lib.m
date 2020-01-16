//#import <CoreFoundation/CoreFoundation.h>
//#import <CoreServices/CoreServices.h>
//#import <QuickLook/QuickLook.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

//  ---------------------------------------------------------------
// |
// |  OPML renderer
// |
//  ---------------------------------------------------------------

NSXMLElement* make(NSString *tag, NSString *text, NSXMLElement *parent) {
	NSXMLElement *div = [NSXMLElement elementWithName:tag];
	if (text) div.stringValue = text;
	[parent addChild:div];
	return div;
}

void attribute(NSXMLElement *parent, NSString *key, NSString *value) {
	[parent addAttribute:[NSXMLElement attributeWithName:key stringValue:value]];
}

NSXMLElement* section(NSString *title, NSString *container, NSXMLElement *parent) {
	make(@"h3", title, parent);
	NSXMLElement *div = make(container, nil, parent);
	attribute(div, @"class", @"section");
	return div;
}

void appendNode(NSXMLElement *child, NSXMLElement *parent) {
	
	if ([child.name isEqualToString:@"head"]) {
		NSXMLElement *dl = section(@"Metadata:", @"dl", parent);
		for (NSXMLElement *head in child.children) {
			make(@"dt", head.name, dl);
			make(@"dd", head.stringValue, dl);
		}
		return;
	}
	
	if ([child.name isEqualToString:@"body"]) {
		parent = section(@"Content:", @"ul", parent);
		
	} else if ([child.name isEqualToString:@"outline"]) {
		if ([child attributeForName:@"separator"].stringValue) {
			make(@"hr", nil, parent);
		} else {
			NSString *desc = [child attributeForName:@"title"].stringValue;
			NSString *xmlUrl = [child attributeForName:@"xmlUrl"].stringValue;
			if (!desc || desc.length == 0)
				desc = [child attributeForName:@"text"].stringValue;
			// refreshInterval
			NSXMLElement *li = make(@"li", desc, parent);
			if (xmlUrl && xmlUrl.length > 0) {
				[li addChild:[NSXMLNode textWithStringValue:@" — "]];
				attribute(make(@"a", xmlUrl, li), @"href", xmlUrl);
			}
		}
		if (child.childCount > 0) {
			parent = make(@"ul", nil, parent);
		}
	}
	for (NSXMLElement *c in child.children) {
		appendNode(c, parent);
	}
}

CFDataRef renderOPML(CFURLRef url, CFBundleRef bundle) {
	NSError *err;
	NSXMLDocument *doc = [[NSXMLDocument alloc] initWithContentsOfURL:(__bridge NSURL*)url options:0 error:&err];
	if (err || !doc) {
		printf("ERROR: %s\n", err.description.UTF8String);
		return nil;
	}
	
	NSXMLElement *html = [NSXMLElement elementWithName:@"html"];
	NSXMLElement *head = make(@"head", nil, html);
	make(@"title", @"OPML file", head);
	
	CFURLRef path = CFBundleCopyResourceURL(bundle, CFSTR("style"), CFSTR("css"), NULL);
	NSString *data = [NSString stringWithContentsOfFile:CFBridgingRelease(path) encoding:NSUTF8StringEncoding error:nil];
	make(@"style", data, head);
	
	NSXMLElement *body = make(@"body", nil, html);
	
	for (NSXMLElement *child in doc.children) {
		appendNode(child, body);
	}
	NSXMLDocument *xml = [NSXMLDocument documentWithRootElement:html];
	return CFBridgingRetain([xml XMLDataWithOptions:NSXMLNodePrettyPrint | NSXMLNodeCompactEmptyElement]);
}
