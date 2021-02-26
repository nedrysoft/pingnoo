# Development Roadmap

This document outlines the development plan for the application.

* Remove the ping engine specific route engine and replace with a single engine that uses a supplied ping engine, this will remove duplicated code and make maintaining much easier.
* ICMPAPI ping engine component needs to be finished, this will allow the application to use the ICMP ping engine built into Windows.
* Remote ping engine, create a ping engine that connects to an "ping agent" which can be local or remote, this would allow reverse analysis to be performed.
* Related the the remote ping engine, a helper application service for linux should be provided along with a ping engine to communicate with it.  The service can be installed and run either as root or with setcap raw socket permissions, this would alleviate the need for the application to run as root.
* Add jitter graphs for each hop.
* Outputs, PDF/PNG etc.
* Favourites, the title of a favourite may contains "/", if it does, then it is used to generate a hierarchical favourite.