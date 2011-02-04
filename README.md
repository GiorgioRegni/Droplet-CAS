DROPLET-CAS
===========

## Content Addressable Storage for Droplet compatiable cloud service providers.

Droplet-CAS uses efficient data deduplication checksum algorithms to provide a simple web service API for
backup and other use cases where content is relatively fixed, such as documents stored for compliance
 with government regulations.

Content addressable storage means that the system provides key for each file that is a function of its content.
Files are read back from the system by using this key, not by file name. It's up to the client to store the mapping
between a file and it's uniquely generated key.

## API

 * WRITE
 * READ
 * DELETE


