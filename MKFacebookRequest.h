//
//  MKFacebookRequest.h
//  MKAbeFook
//
//  Created by Mike on 12/15/07.
/*
 Copyright (c) 2009, Mike Kinney
 All rights reserved.
 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 Neither the name of MKAbeFook nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#import <Cocoa/Cocoa.h>
#import "MKFacebook.h"
#import "MKFacebookSession.h"
#import "MKFacebookResponseError.h"

extern NSString *MKFacebookRequestActivityStarted;
extern NSString *MKFacebookRequestActivityEnded;


/*!
 @enum MKFacebookRequestType
 */
enum MKFacebookRequestType
{
	MKFacebookRequestTypePOST,
	MKFacebookRequestTypeGET
};
typedef int MKFacebookRequestType;


/*!
 @enum MKFacebookRequestResponseFormat
 */
enum MKFacebookRequestResponseFormat
{
	MKFacebookRequestResponseFormatXML,
	MKFacebookRequestResponseFormatJSON
};
typedef int MKFacebookRequestResponseFormat;



/*!
 @class MKFacebookRequest
 MKFacebookRequest handles all requests to the Facebook API.  Requests are sent asynchronously and responses (XML or JSON) via the MKFacebookRequest delegate methods.
 
 To send a request you must set the method property and supply any parameters via the parameters property. The parameters dictionary you provide should not inlcude the access_token key, as this is automatically added to each request.
 
 
 Note: To to maintain backwards compatibility with older versions of MKAbeFook, you may use the key "method" in your parameters dictionary instead of setting the method property.
  
 MKFacebookRequest should support most of the legacy REST API methods, for a list of supported methods and their parameters visit https://developers.facebook.com/docs/reference/rest/
 
 This class will post notifications named "MKFacebookRequestActivityStarted" and "MKFacebookRequestActivityEnded" when network activity starts and ends.  You are responsible for adding observers for handling the notifications.
 
  @see MKFacebookRequestDelegate
  @see MKFacebookRequestQueue
  @version 0.7 and later
 */
@interface MKFacebookRequest : NSObject {
	NSURLConnection *theConnection; //internal connection used if object is used multiple times.
	BOOL _requestIsDone; //dirty stupid way of trying to prevent crashing when trying to cancel the request when it's not active.  isn't there a better way to do this?
	NSURL *requestURL;
	int _requestAttemptCount;
	MKFacebookSession *_session;

	//exposed via properties
	id delegate;
	SEL selector;
	NSString *method;
	NSMutableDictionary *parameters;
	NSString *rawResponse;
	MKFacebookRequestType urlRequestType;
	MKFacebookRequestResponseFormat responseFormat;
    int numberOfRequestAttempts;
	BOOL displayAPIErrorAlerts;
    NSTimeInterval connectionTimeoutInterval;
	NSMutableData *_responseData;

    
	//default selectors
	SEL defaultResponseSelector;
	SEL defaultErrorSelector;
	SEL defaultFailedSelector;
	
	//deprecated selectors
	SEL deprecatedResponseSelector;
	SEL deprecatedErrorSelector;
	SEL deprecatedFailedSelector;
}


/*! @name Properties */
//@{
/*!
 @brief Delegate to be passed response from request.
 */
@property (nonatomic, assign) id delegate;

/*!
 @brief Custom selector to accept response from Facebook.
 
 You can specify a custom selector to be called when the request receives a successful response from Facebook. Your selector should accept a single (id) parameter. 
 
 Note: If an error is encountered the MKFacebookRequestDelegate methods will be called.
 */
@property (nonatomic, assign) SEL selector;

/*!
 @brief Facebook method to call.
 
 See https://developers.facebook.com/docs/reference/rest/ for a list of methods that can be called.
 
 @version 0.9 and later
 */
@property (retain, nonatomic) NSString *method;


/*!
 @brief The parameters for the request.
 
 See https://developers.facebook.com/docs/reference/rest/ documentation on parameters for Facebook methods.
 
 @see setParameters:
 */
@property (retain, nonatomic) NSDictionary *parameters;


/*!
 @brief Unparsed response from Facebook.
 
 Contains unparsed XML or JSON result from Facebook. Use responseFormat property to specify which type of response you want returned from Facebook. Only available when using asynchronous requests.
 
 @see responseFormat
 
 @version 0.9 and later
 
 */
@property (readonly) NSString *rawResponse;


/*!
 @brief The type of request to perform. (Get or Post)
 
 Default is MKFacebookRequestTypePOST
 */
@property (nonatomic, assign) MKFacebookRequestType urlRequestType;


/*!
 @brief Set the response format type, XML or JSON.  XML is default.
 
 Accepts MKFacebookRequestResponseFormatXML or MKFacebookRequestResponseFormatJSON to specify response type.
 
 Both XML and JSON will automatically be parsed to a NSDictionary or NSArray and returned via the appropriate delegate method. For direct access to the unparsed response see the rawResponse property.
 
 @version 0.9 and later

 @see responseFormat
 @see rawResponse
 */
@property (nonatomic, assign) MKFacebookRequestResponseFormat responseFormat;


/*!
 @brief Set the number of times to attempt a request before giving up.
 
 Sets how many times the request should be attempted before giving up.  Note: the delegate will not receive notification of a failed attempt unless all attempts fail.  Default is 5.
 */
@property (readwrite) int numberOfRequestAttempts;


/*!
 @brief Display API Error alert windows.
 
 Sets whether or not instance should automatically display error windows when network connection or xml parsing errors are encountered.  Default is yes.
 */
@property (readwrite) BOOL displayAPIErrorAlerts;


/*!
 @brief How long to wait before aborting the request.
 
 How long the connection should wait before giving up. Default is 30 seconds.
 
 @version 0.9 and later
 */
@property NSTimeInterval connectionTimeoutInterval;

//@}

#pragma mark init methods
/*! @name Creating and Initializing
 *
 */
//@{

/*!
 @brief Setup new MKFacebookRequest object.
 
 @param aDelegate The object that will receive the information returned by Facebook.  This should implement -(void)facebookResponseReceived:(id)response to handle data returned from Facebook.  Set a selector to have valid responses sent elsewhere.
 @version 0.9 and later
 */
+ (id)requestWithDelegate:(id)aDelegate;




/*!
 @brief Setup new MKFacebookRequest object.
 
 @param aDelegate The object that will receive the response returned by Facebook.
 @param aSelector Method in delegate object to be called and passed the valid response from Facebook.  This method should accept an (id) as an argument. Responses that contain an error will not be sent to this selector. Use the appropriate delegate methods for handling error responses.
 
 @see MKFacebookRequestDelegate
 
 @version 0.9 and later
 */
+ (id)requestWithDelegate:(id)aDelegate selector:(SEL)aSelector;



- (id)init;



/*!
 @brief Setup new MKFacebookRequest object.
 
 @param aDelegate The object that will receive the information returned by Facebook.
 @param aSelector Method in delegate object to be called and passed the valid response from Facebook.  This method should accept an (id) as an argument. Responses that contain an error will not be sent to this selector. Use the appropriate delegate methods for handling error responses.

 @see MKFacebookRequestDelegate
 
 @version 0.9 and later
 */
- (id)initWithDelegate:(id)aDelegate selector:(SEL)aSelector;


//@}

#pragma mark -


#pragma mark Instance Methods

/*! @name Preparing and Sending Asynchronous Requests
 *
 */
//@{

/*!
 @brief Pass in a NSDictionary of parameters for your request.
 
 @param params NSDictionary containing parameters for requested method.  
 
 See https://developers.facebook.com/docs/reference/rest/ for documentation regarding parameters for the request you are making.
 
 @verbatim
 NSMutableDictionary *parameters = [[NSMutableDictionary alloc] init];
 
 //set up parameters for request
 [parameters setValue:[fbConnection uid] forKey:@"uid"];
 
 //lists can be either comma separated strings or arrays containing strings, the next two lines are both valid
 [parameters setValue:[NSArray arrayWithObjects:@"first_name",@"last_name",nil] forKey:@"fields"];
 [parameters setValue:@"first_name,last_name" forKey:@"fields"];
 
 //add parameters to request
 [request setParameters:parameters];
  
 [parameters release];
 @endverbatim
 
 @warning Using a NSArray instead of a comma separated string for a list parameter is only available in 0.9 and later.
 
 @version 0.7 and later
 */
- (void)setParameters:(NSDictionary *)params;


/*!
 @brief Generates the appropriate URL and based on method and parameters for request.  Sends request to Facebook.
 
 MKFacebookRequest will automatically attempt the request again if any of the following errors are encountered:
 - max number of requests allowed reached
 - unknown error
 - service unavailable
 
 The result will be passed to the delegate / selector that were assigned to this object. Make sure you have either set the method property or have set the parameters value with a NSDictionary containing a 'method' key before calling sendRequest.
 
 @verbatim
 NSMutableDictionary *parameters = [[NSMutableDictionary alloc] init];
 
 //set up parameters for request
 [parameters setValue:@"users.getInfo" forKey:@"method"];
 [parameters setValue:[fbConnection uid] forKey:@"uid"];
 
 //lists can be either comma separated strings or arrays containing strings, the next two lines are both valid
 [parameters setValue:[NSArray arrayWithObjects:@"first_name",@"last_name",nil] forKey:@"fields"];
 [parameters setValue:@"first_name,last_name" forKey:@"fields"];
 
 //add parameters to request
 [request setParameters:parameters];
 
 //send the request
 [request sendRequest];
 
 [parameters release];
 @endverbatim
 
 @warning Raises NSException if the method property has not been set or parameters has not been set with NSDictionary containing a 'method' key.
 
 @see method
 @see setParameters:
 @see sendRequestWithParameters:
 @see sendRequest:withParameters:
 @see MKFacebookRequestDelegate

 @version 0.7 and later
 */
- (void)sendRequest;

/*!
  @brief Sets the parameters before sending the request.
 
 @deprecated Use sendRequest:withParameters: instead.
 
 @see sendRequest:withParameters:
 */
- (void)sendRequestWithParameters:(NSDictionary *)params;

/*!
  @brief Sets method and parameters before sending the request.
 
 @param aMethod Facebook method to call.
 
 @param params NSDictionary of parameters to pass to method. Does not need to contain a 'method' key.
 
 Sets the method and parameters before sending the request.
 
 See sendRequest for details about what happens when a request is sent to Facebook.
 
 @see sendRequest

 @version 0.9 and later
*/
- (void)sendRequest:(NSString *)aMethod withParameters:(NSDictionary *)params;

//@}



/*! @name Synchronous Requests
 *
 */
//@{

/*!
 @brief Generates a full URL including a signature for the method name and parameters passed in.  
 
 @param aMethodName Full Facebook method name to be called.  Example: users.getInfo
 @param params NSDictionary containing parameters and values for the method being called.  They keys are the parameter names and the values are the arguments.
 
 This method will automatically include all parameters required by every Facebook method.  Parameters you do not need to include are "v", "api_key", "format", "session_key", and "call_id".  See official Facebook documentation for all other parameters available depending on the method you are calling.  As of 0.7 this method is considered deprecated, use generateFacebookURL: instead.
 
 @result Returns complete NSURL ready to be sent to the Facebook API.
 
 @see generateFacebookURL:
 @see fetchFacebookData:
 
 */
- (NSURL *)generateFacebookURLForMethod:(NSString *)aMethodName parameters:(NSDictionary *)params;


/*!
 @brief Performs a synchronous request using URL generated by generateFacebookURL:parameters: or generateFacebookURL:
 
 @param theURL URL generated by generateFacebookURL:parameters: or generateFacebookURL:
 
 Initiates a synchronous request to Facebook.
 
 @result Returns NSXMLDocument that was returned from Facebook.  Returns nil if a network error was encountered.
 
 @see generateFacebookURL:parameters:
 @see generateFacebookURL:
 
 @version 0.7 and later
 */
- (id)fetchFacebookData:(NSURL *)theURL;
//@}


/*! @name Canceling a Request
 *
 */
//@{
/*!
 @brief Cancels a request if in progress.
 
 Cancels the current asynchronous request if one is in progress.  Synchronous requests cannot be cancelled.
 @version 0.7 and later
 */
- (void)cancelRequest;
//@}


#pragma mark -


@end



/*!
 @protocol MKFacebookRequestDelegate
 MKFacebookRequestDelegate is responsible for handling the XML or JSON that Facebook returns. Your delegate should implement either the default response methods or specify your own if you assigned a custom selector to your MKFacebookRequest.
 
 If you do not assign your MKFacebookRequest instance a custom selector, use the facebookRequest:responseReceived: method to catch responses.
 
 If you assigned your own selector it must accept a single (id) argument.
 
 Use facebookRequest:errorReceived: and facebookRequest:failed: to handle errors.
 */
@protocol MKFacebookRequestDelegate


/*! @name Receive Valid Response
 *
 */
//@{

/*!
  @brief Facebook returned a valid response.
 
 Called when Facebook returns a valid response and passes the response returned by Facebook.  The response will be a NSXMLDocument if the request type is XML or a NSDictionary or NSArray if the request type is JSON. If you want the responses sent elsewhere assign the request a selector.
 
 @param response The parsed response from Facebook (either a NSDictionary or NSArray).
 
 @see facebookRequest:responseReceived:
 
 @version 0.8 and later
 
 @deprecated Version 0.9
 */
@optional
- (void)facebookResponseReceived:(id)response;


/*!
 @brief Facebook returned a valid response.
 
 Called when Facebook returns a well formed JSON or XML response that does not contain errors from the Facebook API. If your request was missing required parameters you will not receive the response in this method because it will contain errors from the Facebook API; instead the error response will be sent to facebookRequest:errorReceived:

 If your request specified a JSON responseFormat the response passed to this method will be either a NSDictionary or NSArray. 
 
 If your request specified a XML responseFormat you will receive an unparsed NSXMLDocument object. See NSXMLElementAdditions category for additional XML parsing methods.
 
 @param request The request that returned an error from Facebook.
 
 @param response The response from Facebook. The response could be either a NSDictionary, NSArray, or NSXMLDocument depending on the responseFormat set in the request. 
 

 @see MKFacebookRequest
 @see responseFormat
 @see facebookRequest:errorReceived:
 
 @version 0.9 and later
 */
@optional
- (void)facebookRequest:(MKFacebookRequest *)request responseReceived:(id)response;
//@}



/*! @name Reveive Error Responses
 *
 */
//@{


/*!
 @brief Facebook returned a response containing an error.
 
 Called when an error is returned by Facebook.  Passes response returned by Facebook.
 
 @param error The error response from Facebook.
 
 @see facebookRequest:errorReceived:
 
 @version 0.8 and later
 
 @deprecated Version 0.9
 */
@optional
- (void)facebookErrorResponseReceived:(id)errorResponse;


/*!
 @brief Facebook returned a response containing an error.
 
 Called when an error is returned by Facebook.  Passes the request that caused the error along with details of the error.

 @param request The request that returned an error from Facebook.
 
 @param error The error Facebook returned.
 
  
 @version 0.9 and later
 */

@optional
- (void)facebookRequest:(MKFacebookRequest *)request errorReceived:(MKFacebookResponseError *)error;
//@}

/*! @name Request Failed */
//@{

/*!
 @brief Request has failed.
 
 Called when the request could not be made.  Passes NSError containing information about why it failed (usually due to NSURLConnection problem).
 
 @param Error explaining the failure.
 
 @see facebookRequest:failed:
 
 @version 0.8 and later
 
 @deprecated Version 0.9
 */
@optional
- (void)facebookRequestFailed:(id)error;


/*!
 @brief Request has failed.
 
 Called when the request could not be made. The error will not contain any information from Facebook because the request never got that far.  Passes NSError containing information about why it failed (usually due to NSURLConnection problem).
 
 @param request The request that failed.
 
 @param error Error explaining the failure.
 
 @version 0.9 and later
 */
@optional
- (void)facebookRequest:(MKFacebookRequest *)request failed:(NSError *)error;
//@}


/*! @name Request Progress
 *
 */
//@{
/*!
 @brief Receive progress of an a request.
 
 Called as the body of a POST request is transmitted. Direct wrapper for the internal NSURLConnection delegate method - connection:didSendBodyData:totalBytesWritten:totalBytesExpectedToWrite.
 
 @param request The request sending the message.
 
 @param bytesWritten The number of bytes written in the latest write.
 
 @param totalBytesWritten The total number of bytes written in the latest write.
 
 @param totalBytesExpectedToWrite The number of bytes the request expects to write.
 
 @version 0.9 and later (Mac OS X v10.6 required)
 */
@optional
- (void)facebookRequest:(MKFacebookRequest *)request bytesWritten:(NSUInteger)bytesWritten 
												totalBytesWritten:(NSUInteger)totalBytesWritten 
										totalBytesExpectedToWrite:(NSUInteger)totalBytesExpectedToWrite;
//@}


@end