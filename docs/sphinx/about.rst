*************
About Pingnoo
*************

Pingnoo is an open-source cross-platform application for analysing and measuring the round trip time (latency) between two hosts.

Using ICMP_ (`Internet Control Message Protocol <ICMP_>`_) packets, Pingnoo discovers the route to a host by sending repeated `ICMP <icmp_>`_ packets and incrementing the `time-to-live <TTL_>`_ (TTL_) field, as the packet passes through routers (Hops_) the TTL_ field is decremented, if the value reaches zero then the router that is currently processing that packet will usually respond with a time exceeded message to the origininating host.

.. note::
  Not all hops will respond with time exceeded messages, depending on their configuration they may also prioritise other traffic over ICMP_ packets or they may simply just silently drop the packet.

Pingnoo tracks all ICMP_ packets it transmits and is able to pair ICMP_ responses with the sent requests, this allows the `round-trip-time <RTT_>`_ (RTT_) to be calculated not only between the two endpoints, but also the intermediate hops_ between the hosts.

This information is plotted with graphs to provide an instant visual overview of a route and can be used to identify network issues, not just locally but at any point between the originating computer and the target.

Why Pingnoo?
************

Pingnoo is intended to be used by anybody with an interest in the performance of their network connections, from home users to enterprise users), the application has been designed to be quick and easy to use; whether you are interested in gaming performance issues or whether you are trying to diagnose enterprise routing issues.

Features
********

Although currently early in development, the core application functionality is fully usable and the capabilities of the software will grow over time.

- `100% open-source software <repo_>`_, licensed under the `GPLv3 <license_>`_.
- Cross-platform, runs under macOS, Windows and Linux.
- Extensible, the main application binary is very small and only serves as a loader for components (aka plugins), the actual functionality of the software is provided by components which makes the design ultra-flexible and allows third parties to produce their own components to further enhance the software.
- Produces beautiful graphs showing the history of each hop.
- Built in support to redact host name and IP addresses.  Ships with host maskers for the (detected) public IP address and also a regular expression based masker which allows complex masking rules to be created, producing outputs without the need to further edit the document/image to hide sensitive information.

.. figure:: /images/about-pingnoo-application-screenshot-linux.png
   :alt: Linux screenshot
   :class: with-shadow

   The Linux version showing the historical RTT to cloudflare for the past 30 minutes.

Getting started
***************

If you've read this far, then all that remains is to head to the releases_ section of the `project repository <repo_>`_ and download the appropriate version for your operating system, within minutes you will be producing detailed information about your network connection!

.. _icmp: https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
.. _latency: https://en.wikipedia.org/wiki/Latency_(engineering)
.. _ttl: https://en.wikipedia.org/wiki/Time_to_live
.. _hops: https://en.wikipedia.org/wiki/Hop_(networking)
.. _rtt: https://en.wikipedia.org/wiki/Round-trip_delay
.. _license: https://github.com/nedrysoft/pingnoo/blob/develop/LICENSE
.. _repo: https://github.com/nedrysoft/pingnoo
.. _releases: https://github.com/nedrysoft/pingnoo/releases