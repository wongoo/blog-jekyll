---
author: admin
comments: true
date: 2012-07-17 09:20:33+00:00
excerpt: Resolving multiple-pages tiff converting error EOFException  using iText
layout: post
slug: resolving-multiple-pages-tiff-converting-error-using-itext
title: Resolving multiple-pages tiff converting error using iText
wordpress_id: 303
categories:
- Experience
tags:
- iText
- solution
- tiff
---

The solution in our project is using iText to convert tiff to PDF. It's running OK for single-page and multiple-pages tiff when testing. But the code doesn't work for customer's tiff files when UAT.

**The error message:**


java.io.EOFException
at com.itextpdf.text.pdf.RandomAccessFileOrArray.readFully(RandomAccessFileOrArray.java:292)


After debug and see the source code , I found that ?the TIFFDirectory(s, page) will called to do some initialization everytime when calling?TiffImage.getTiffImage(RandomAccessFileOrArray, index), which seems would cause wrong index reading??for the second time for a same?RandomAccessFileOrArray or a same?InputStream object?.



**So,my solution is creating a new RandomAccessFileOrArray ?object ?using byte array every time when calling?TiffImage.getTiffImage(RandomAccessFileOrArray, index).**



**The Type of the Tiff file which has error:**


TIFFConstants.COMPRESSION_OJPEG
TIFFConstants.PHOTOMETRIC_MINISBLACK


**The Java code:**

    
    
    public static void tiff2Pdf(InputStream is, OutputStream os, String inputFileName) {
    	Document document = new Document();
    	try {
    		PdfWriter writer = PdfWriter.getInstance(document, os);
    		writer.setStrictImageSequence(true);
    		document.open();
    		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
    		byte[] bytes = new byte[1024];
    		int len = 0;
    		while ((len = is.read(bytes)) > 0) {
    			byteArrayOutputStream.write(bytes, 0, len);
    		}
    		byte[] byteArray = byteArrayOutputStream.toByteArray(); //get bytes array
    		RandomAccessFileOrArray ra = new RandomAccessFileOrArray(byteArray);
    		int pages = TiffImage.getNumberOfPages(ra); // get page count
    		float pageWidth = document.getPageSize().getWidth();
    		for (int i = 1; i <= pages; i++) {
    			Image image = TiffImage.getTiffImage(new RandomAccessFileOrArray(byteArray), i); //new
    			float imageWidth = image.getWidth();
    			if (pageWidth < imageWidth) {
    			image.scalePercent((pageWidth - 50) / imageWidth * 100);
    		}
    		image.setAlignment(Image.MIDDLE);
    		document.add(image);
    		document.newPage();
    		}
    	} catch (DocumentException e) {
    		logger.error(e.getMessage(), e);
    	} catch (IOException e) {
    		logger.error(e.getMessage(), e);
    	} finally {
    		try {
    			document.close();
    		} catch (Exception e) {
    		}
    	}
    }
    
    

