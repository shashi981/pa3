/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  pair<int,int> ul(0,0);
  stats *s = new stats(imIn);
  root = buildTree(*s, ul, imIn.width(), imIn.height(), tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  if(s.getVar(ul,w,h) <= tol || (w == 1 && h == 1)){
    return new Node(s, ul, w, h);
  }    
  Node * nod = new Node(s,ul,w,h);
  Node * tempNW, *tempNE, *tempSW, *tempSE;
  pair<int,int> TL, TR, BL, BR;
  TL = ul;
  TR.first = ul.first;
  TR.second = h/2;
  BL.first = w/2;
  BL.second = ul.second;
  BR.first = w/2;
  BR.second = h/2;
  tempNW = buildTree(s,TL,w/2,h/2,tol); 
  tempNE = buildTree(s,TR,w/2,h/2,tol);
  tempSW = buildTree(s,BL,w/2,h/2,tol);
  tempSE = buildTree(s,BR,w/2,h/2,tol);
  if(tempNW->var <= tol)
    nod->NW = tempNW;
  if(tempNW->var <= tol)
    nod->NE = tempNE;
  if(tempNW->var <= tol)
    nod->SW = tempSW;
  if(tempNW->var <= tol)
    nod->SE = tempSE;  
  return nod; 
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG *im = new PNG(root->width,root->height);
  Imager(root,*im);
  return *im;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  cleaner(root);  
}

void SQtree::copy(const SQtree & other) {
  copier(root,other.root);
}

int SQtree::size() {
  return len(0, root);
}

void SQtree::cleaner(Node * nod) {
  if(nod == NULL)
    return;
  if(nod->NW != NULL)
    cleaner(nod->NW);
  if(nod->NE != NULL)
    cleaner(nod->NE);
  if(nod->SE != NULL)
    cleaner(nod->SE);
  if(nod->SW != NULL)
    cleaner(nod->SW);
  free(nod);
}

int SQtree::len(int count, Node * nod) {
  if(nod == NULL)
    return 0 + count;
  if(nod->NW != NULL)
    count = len(count, nod->NW);
  if(nod->NE != NULL)
    count = len(count, nod->NE);
  if(nod->SE != NULL)
    count = len(count, nod->SW);
  if(nod->SW != NULL)
    count = len(count, nod->SE);
  return 1 + count;
}

SQtree::Node * SQtree::copier(Node* nod, Node* copiedNode) {
  if(nod == NULL)
    return NULL;
  if(nod->NW != NULL)
    copiedNode->NW = copier(nod->NW,copiedNode->NW);
  if(nod->NE != NULL)
    copiedNode->NE = copier(nod->NE,copiedNode->NE);
  if(nod->SE != NULL)
    copiedNode->SW = copier(nod->SW,copiedNode->SW);
  if(nod->SW != NULL)
    copiedNode->SE = copier(nod->SE,copiedNode->SE);
  return new Node(nod->upLeft,nod->width,nod->height,nod->avg,nod->var);
}

void SQtree::Imager(Node* nod, PNG im) {
  if(nod == NULL)
    return;
  for(int x = nod->upLeft.first; x < nod->width; x++) {
    for(int y = nod->upLeft.second; y < nod->height; y++) {
      RGBAPixel *pix = im.getPixel(x,y);
      *pix = nod->avg;
    }
  }
  if(nod->NW != NULL){
    Imager(nod->NW, im);
  }
  if(nod->NE != NULL){
    Imager(nod->NE, im);
  }
  if(nod->SW != NULL){
    Imager(nod->SW, im);
  }
  if(nod->SW != NULL){
    Imager(nod->SE, im);
  }
}