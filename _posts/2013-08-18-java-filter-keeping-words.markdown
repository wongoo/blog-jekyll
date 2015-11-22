---
author: admin
comments: true
date: 2013-08-18 09:45:02+00:00
excerpt: Java过滤保留字字符串处理
layout: post
slug: java-filter-keeping-words
title: Java过滤保留字字符串处理
wordpress_id: 482
categories:
- Experience
tags:
- algorithm
- java
---

**1. 先定义一个保留字类：**

    
    import java.util.HashMap;
    import java.util.Set;
    
    /**
     * @author Geln Yang
     * 
     */
    public class KeepingWordMap extends HashMap {
    
    	private static final long serialVersionUID = 1L;
    
    	private static KeepingWordMap dbKeepingWordMap = new KeepingWordMap();
    
    	public static KeepingWordMap getDbKeepingWordMap() {
    		return dbKeepingWordMap;
    	}
    
    	private boolean wordEnd = false;
    
    	public boolean isWordEnd() {
    		return wordEnd;
    	}
    
    	public void addWord(String word) {
    		System.out.println("add keeping word:" + word);
    		addWord(word.toCharArray(), 0);
    	}
    
    	public void removeWord(String word) {
    		System.out.println("remove keeping word:" + word);
    		removeWord(word.toCharArray(), 0);
    	}
    
    	public void addWord(char[] charArray, int index) {
    		if (index >= charArray.length) {
    			wordEnd = true;
    			return;
    		}
    		char c = charArray[index];
    		KeepingWordMap subMap = get(c);
    		if (subMap != null) {
    			subMap.addWord(charArray, ++index);
    		} else {
    			subMap = new KeepingWordMap();
    			put(c, subMap);
    			subMap.addWord(charArray, ++index);
    		}
    	}
    
    	public void removeWord(char[] charArray, int index) {
    		if (index >= charArray.length) {
    			wordEnd = false;
    			return;
    		}
    		char c = charArray[index];
    		KeepingWordMap subMap = get(c);
    		if (subMap != null) {
    			if (index < charArray.length - 1) {
    				subMap.removeWord(charArray, ++index);
    				if (subMap.isEmpty() && !subMap.wordEnd) {
    					remove(c);
    				}
    			} else if (index == charArray.length - 1) {
    				if (subMap.wordEnd) {
    					if (subMap.isEmpty()) {
    						remove(c);
    					}
    					subMap.removeWord(charArray, ++index);
    				}
    			}
    		} else {
    			remove(c);
    		}
    	}
    
    	public void printMap() {
    		System.out.println("-------------------");
    		printMap("");
    	}
    
    	public void printMap(String pre) {
    		Set keys = keySet();
    		for (Character c : keys) {
    			KeepingWordMap subMap = get(c);
    			if (subMap != null) {
    				if (subMap.wordEnd) {
    					System.out.println(pre + c);
    				}
    				subMap.printMap(pre + c);
    			} else {
    				System.out.println(pre + c);
    			}
    		}
    
    	}
    
    }
    

**2. 测试保留字对象：**

    
    public void testAddRemove() {
    		KeepingWordMap map = new KeepingWordMap();
    		map.addWord("中国");
    		map.addWord("中国人");
    		map.addWord("中国人民");
    
    		map.printMap();
    
    		map.removeWord("中国人");
    		map.removeWord("中国");
    		map.printMap();
    
    		map.addWord("中国人");
    		map.printMap();
    
    		map.addWord("中国魂");
    		map.printMap();
    
    		map.removeWord("中国人民");
    		map.printMap();
    
    		map.addWord("中行");
    		map.printMap();
    
    		map.addWord("a");
    		map.addWord("ab");
    		map.addWord("abc");
    		map.addWord("abcd");
    		map.printMap();
    
    		map.removeWord("ab");
    		map.printMap();
    	}
    

**3. 过滤保留字字符串处理算法：**

    
    public char[] process(char[] str, KeepingWordMap map) {
    		if (str == null) {
    			return null;
    		}
    		char[] out = Arrays.copyOf(str, str.length);
    		int previousIndex = 0;
    		for (int i = 0; i < str.length; i++) {
    			do {
    				previousIndex = i;
    				i = findNextProcessIndex(str, i, map);
    			} while (previousIndex != i && i < str.length);
    
    			if (i < str.length) {
    				out[i] = process(str[i]);
    			}
    		}
    		return out;
    	}
    
    	private int findNextProcessIndex(char[] str, int index, KeepingWordMap map) {
    		KeepingWordMap subMap = map.get(str[index]);
    		int end = index;
    		while (subMap != null && !subMap.isEmpty() && index < str.length) {
    			if (subMap.isWordEnd()) {
    				end = index + 1;
    			}
    			index++;
    			if (index >= str.length) {
    				break;
    			}
    			subMap = subMap.get(str[index]);
    
    		}
    		if (subMap != null && subMap.isWordEnd()) {
    			end = index + 1;
    		}
    		return end;
    	}
    

