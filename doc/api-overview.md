<!--
  --  Copyright (C) 2015-2016 IC Manage Inc.
  --
  --  See the file 'COPYING' for license information.
  -->
# VelStor API Overview

## Introduction

### Purpose

To ensure the VelStor API conforms to the HTTP 1.1 standard; that it has a uniform design; that it meets widely held expectations for a RESTful API; and that any deviations are documented.

### Terminology

Term | Definition
--- | ---
Archetype: Collection | A URI which represents a collection of entities, such as a filesystem directory.
Archetype: Controller | A URI which represents an operation on a resource, one which does not map to an HTTP method.
Archetype: Document | A URI which represents a unitary entity, such as a file (as opposed to a directory).
Archetype: Store | A URI which represents a repository for resources, but which does not create a URI for the resources it holds.
[HATEOAS](https://en.wikipedia.org/wiki/HATEOAS) | Hypertext as the Engine Of Application State, an aspect of REST.
[REST](https://en.wikipedia.org/wiki/Representational_state_transfer) | REpresentational State Transfer, an architectural style.

The terms for resource archetypes (collection, controller, document, store) are taken from Masse, below. They are useful when discussing HTTP and extended methods.

### Normative References 

IETF [*Hypertext Transfer Prototocol (HTTP/1.1): Semantics and Content*](http://tools.ietf.org/html/rfc7231)

### Non-normative References 

Fielding, Roy [*It is okay to use POST*](http://roy.gbiv.com/untangled/2009/it-is-okay-to-use-post) 

Fielding invented REST in his PhD thesis. In the blog post above, he discusses the approprate use of GET, PUT and POST.

Masse, Mark [*REST API Design Rulebook*](http://shop.oreilly.com/product/0636920021575.do)

Chapters two through five of Masse provide a concise summary of generally accepted REST practice. VelStor ignores portions of the book dealing with hypermedia handling or WRML.

StackOverflow [exposing operations on resources RESTfully](http://stackoverflow.com/questions/9719693/)

Comment '4' to the question advocates giving processes their own URI rather than overloading POST.

Square [*Square Connect API Conventions*](https://docs.connect.squareup.com/api/connect/v1/#overview)

Twitter [*REST APIs*](https://dev.twitter.com/rest/public)

Webber, Jim et al [*REST in Practice, Hypermedia and Systems Architecture*](http://shop.oreilly.com/product/9780596805838.do) 

Wikipedia [*HATEOAS*](https://en.wikipedia.org/wiki/HATEOAS)

Wikipedia [*REST*](https://en.wikipedia.org/wiki/Representational_state_transfer)

Paragraphs of this document which begin with *tbd* or *rationale* are informative rather than normative.

## Architecture

The VelStor API is a web service which exchanges JSON documents over HTTP. It generally follows the [REST architectural style](https://en.wikipedia.org/wiki/Representational_state_transfer) with exceptions noted below.

### Conformance

The web service conforms to HTTP 1.1 as
specified in IETF RFC 2730 through 2737. In particular, the VelStor API architecture
honors the requirements of [IETF RFC 2731](http://tools.ietf.org/html/rfc7231).

### HTTP

The VelStor API uses HTTP error codes as required by the HTTP standard. Beyond that, HTTP is treated as a transport. HTTP error codes indicate the success or failure of message conveyance. The success or failure of the web service operation is indicated in the JSON response body.

For example, a client presenting invalid credentials to the VelStor vCNC server will receive "401 - Not Authorized" in response. However, a client having appropriate credentials to execute an HTTP method on the server, but insufficient credentials to perform the corresponding operation on a TRQ, will receive an "200" HTTP response with an error code in the response body.

The following table summarizes HTTP errors in VelStor API:

HTTP Code | Description
--- | ---
200 | Issued when none of the other HTTP errors apply.  Indicates that the request was received, authorized, well-formed, and processed. Operational status, including error codes, is indicated in the response body.
400 | Issued when a JSON request body is ill-formed or otherwise invalid.
401 | Issued when the supplied JWT token is not valid, or when the vCNC user does not have permission to perform the web operation. Does not apply to an operation on the TRQ for which permission was denied.
404 | Issued when a resource, specified in the URL path, unexpectedly does not exist. Does not apply to resources specified in a request body.
504 | Issued when the specified TRQ does not exist or does not respond.


### REST

The [REST architectural style](https://en.wikipedia.org/wiki/Representational_state_transfer)
has three key elements:

* Resources are represented by URIs
* Operations on resources are represented as HTTP methods
* Response bodies define the currently valid operations on a resource.

The last element is the "state transfer" portion of "REprestational State
Transfer" (REST). It is also known as "Hypermedia As The Engine Of Application
State"
([HATEOAS](https://en.wikipedia.org/wiki/HATEOAS)).

The VelStor API meets Webber's notion of a "Level Two Service" (Webber, page 38).

## Policies and Conventions

### Resources represented as URIs

With only a few exceptions, an API call specifies in the URL the primary resource on which it operates.

Exceptions include API calls which represent a vector of independent operations to be performed in parallel, for example, vector meta-data copy of a TRQ namespace.

### Hierarchical names are URL-encoded

Several types of VelStor resources have hierarchical names, represented /by/slash/separation. Such hierarchical names are url-encoded before being placed in the URI hierarchy.

For example, the URI

https://api.acme.com/v1/trq/workspaces/9/%2Faustin%2Fmarketing%2Fenigma

refers to the VelStor workspace "/austin/marketing/enigma" on the TRQ having ID "9".

*rationale*: VelStor hierarchical names may contain characters which are not valid in URLs. Encoding the hierarchial name as a single URI element simplifies the design of extended operations because the name becomes a single URI element.

*tbd*: The question of whether hierarchical names are url-encoded in JSON, or whether they are encoded using JSON-specific mechanisms, is still open.

### Operations as HTTP methods

VelStor follows the usual conventions:

* GET retreives information without altering server state.
* DELETE removes a resource or collection of resources.
* PUT replaces an existing resource.
* POST creates a resource which did not previously exist.

POST is used in cases where, depending on context, either PUT or POST may apply. For example, a POST operation may update an existing resource and also may create the resource if it does not exist.

### Extended Methods

In some cases the [standard HTTP request methods](http://tools.ietf.org/html/rfc7231#section-4.3) are insufficient to express all of the available operations on a resource. A *controller URI* (Masse, page 35) is a widely adaopted solution. The extended method name becomes the last element of the URI.

This solution generates parsing ambiguities in even the simplest
situations. Consider the operations:

  GET /pets/findByName?...
  GET /pets/{pet-name}

There is clearly a problem if the pet's name is "findByName". Alternatively,

  GET /findPetByName?...
  GET /pets/{pet-name}

avoids any ambiguity.

VelStor adopts the *controller URI* in situations where a parsing ambiguity does not arise. Otherwise, VelStor puts the operation closer to the beginning of the URL rather than at the end.

  GET  /trq/namespace/{trqhost}/{url-path}/children:
  GET  /trq/namespace/{trqhost}/{url-path}/attributes:
  POST /trq/meta-copy/{trqhost}

*rationale*: While there is still some debate about whether the extended method name should be exposed in the URI or buried in the request JSON body, most of the opprobrium falls on "tunneling" protocols through the request body. Also, putting the extended method name in the URI is consistent with HTTP 1.1 'GET' semantics, therefore enabling a single convention to be used across all HTTP methods. 

### No HTTP 201 on POST

VelStor does not issue a [201 Created](http://tools.ietf.org/html/rfc7231#section-6.3.1) in response to a POST which creates a new resource. Clients may examine the JSON response body to determine the effect of the POST on the server.

### No HATEOAS

The VelStor API does not support HATEOAS.

## Change History

Date | Author | Description
--- | --- | ---
2015-8-8 | Nick | Initial revision
2015-8-11 | Nick | (1) Hierarchical names are now url-encoded, to avoid limiting VelStor file names to those valid in URLs; (2) Extended methods are specified as the final element of a URI, following conventions for "controller resources".
2015-8-14 | Nick | (1) Brought HTTP error handling more in alignment with the standard; (2) New rules for creating extended methods.
2015-8-17 | Nick | (1) Document reference APIs (twitter, square, etc) as non-normative references; (2) Add tags to organize the API; (3) Add operations to manage workspaces.
