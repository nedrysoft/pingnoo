*******************
Installing on Linux
*******************

The latest Linux version of Pingnoo can be found under releases_ on the `GitHub repository <repo_>`_.

Currently Pingnoo is only supplied as a self contained AppImage for linux.  The advantage of AppImages is that they contain all the necessary dependencies required for the software to run, this means that no additional software needs to be installed for the application to run.

Preparing Pingnoo to run
************************

After downloading Pingnoo to an appropriate location, the downloaded files permissions must be set to allow execution, this step must be completed otherwise the application will not run.

.. code-block:: console

   user@machine $ chmod +x Pingnoo.AppImage

Once the permissions hae been changed, the application can be launched directly from the desktop or directly from a shell prompt.

.. code-block:: console

   user@machine $ ./Pingnoo.AppImage

.. warning::
  Ensure that the downloaded applications permissions are set correctly, the application will not not launch otherwise!

Special Requirements
********************

There are additional requirements when using the Linux port of Pingnoo.  Linux by default, prevents applications from using raw sockets as a security measure, this impacts Pingnoo.

In order to use Pingnoo, you either need to run the application with root permissions (not recommended!).

.. code-block:: console

   user@machine $ sudo ./Pingnoo.AppImage

Linux will prompt for the root password, while running as root any files that are created by the application will belong to root, this means that (depending on their permissions) you will not be able to open or delete the created files without root permissions.

Pingnoo has been designed in such a way that different ping engines can be selected.  The system ping command has special permissions that allow it to send ping requests without the need for root permissions, Pingnoo provides a ping engine which makes use of the system ping command, by using this ping engine, Pingnoo does not need root permissions.

.. tip:
  Use the ping command engine to run Pingnoo without needing elevated priiliges.

.. _repo: https://github.com/nedrysoft/pingnoo
.. _releases: https://github.com/nedrysoft/pingnoo/releases