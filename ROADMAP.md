# Development Roadmap

This document outlines the development plan for the application.

* <s>Remove the ping engine specific route engine and replace with a single engine that uses a supplied ping engine, this will remove duplicated code and make maintaining much easier.</s> ***available in release 2021.03.13 or later.***
* ICMPAPI ping engine component needs to be finished, this will allow the application to use the ICMP ping engine built into Windows.
* Remote ping engine, create a ping engine that connects to an "ping agent" which can be local or remote, this would allow reverse analysis to be performed.
* Related the the remote ping engine, a helper application service for linux should be provided along with a ping engine to communicate with it.  The service can be installed and run either as root or with setcap raw socket permissions, this would alleviate the need for the application to run as root.
* <s>Add jitter graphs for each hop.</s> ***available in release 2021.03.21 or later.***
* Outputs, PDF/PNG etc.
* <s>Favourites, the title of a favourite may contains "/", if it does, then it is used to generate a hierarchical favourite.</s> ***available in release 2021.03.13 or later.***