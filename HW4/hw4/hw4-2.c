int sbull_ioctl (struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    int err;
    long size;
    struct hd_geometry geo;

    PDEBUG("ioctl 0x%x 0x%lx\n", cmd, arg);
    switch(cmd) {

      case BLKGETSIZE:
        /* Return the device size, expressed in sectors */
        if (!arg) return -EINVAL; /* NULL pointer: not valid */
        err = ! access_ok (VERIFY_WRITE, arg, sizeof(long));
        if (err) return -EFAULT;
        size = blksize*sbull_sizes[MINOR(inode->i_rdev)]
		/ sbull_hardsects[MINOR(inode->i_rdev)];
	if (copy_to_user((long *) arg, &size, sizeof (long)))
	    return -EFAULT;
        return 0;

      case BLKRRPART: /* reread partition table: can't do it */
        return -ENOTTY;

      case HDIO_GETGEO:
        /*
	 * Get geometry: since we are a virtual device, we have to make
	 * up something plausible. So we claim 16 sectors, four heads,
	 * and calculate the corresponding number of cylinders. We set
	 * the start of data at sector four.
         */
        err = ! access_ok(VERIFY_WRITE, arg, sizeof(geo));
        if (err) return -EFAULT;
        size = sbull_size * blksize / sbull_hardsect;
        geo.cylinders = (size & ~0x3f) >> 6;
	geo.heads = 4;
	geo.sectors = 16;
	geo.start = 4;
	if (copy_to_user((void *) arg, &geo, sizeof(geo)))
	    return -EFAULT;
        return 0;

      default:
        /*
         * For ioctls we don't understand, let the block layer
         * handle them.
         */
        return blk_ioctl(inode->i_rdev, cmd, arg);
    }

    return -ENOTTY; /* unknown command */
}