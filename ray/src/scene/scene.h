//
// scene.h
//
// The Scene class and the geometric types that it can contain.
//

#pragma warning (disable: 4786)


#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <memory>

#include "ray.h"
#include "material.h"
#include "camera.h"
#include "bbox.h"

#include "../vecmath/vec.h"
#include "../vecmath/mat.h"

class Light;
class Scene;

template <typename Obj>
class Node {
  public:
    BoundingBox bb;
    Node<Obj>* left;
    Node<Obj>* right;
    std::vector<Obj> shapes;

    Node(){}

    static Node<Obj>* buildTree( std::vector<Obj>& s, int depth) {
      Node<Obj>* node = new Node();
      node->shapes = s;
      node->left = NULL;
      node->right = NULL;
      node->bb = BoundingBox();

      if ( s.size() <= 0 || depth <= 0) return node;

      if( s.size() == 1){
        node->bb = s[0]->getBoundingBox();
        node->left = new Node();
        node->left->shapes = std::vector<Obj>();
        node->right = new Node();
        node->right->shapes = std::vector<Obj>();
        return node;
      }

      node->bb = s[0]->getBoundingBox();
      // Vec3d mid = ( s[0]->getMidpoint() * (1.0/s.size()) )

      for(int i=1; i < s.size(); i++){
        node->bb.merge(s[i]->getBoundingBox());
        // mid += ( s[i]->getMidpoint() * (1.0/s.size()) );
      }

      Vec3d mid = node->bb.getMid();
      Vec3d tmp = node->bb.getMax() - node->bb.getMin();

      int axis = 0;
      if( abs(tmp[1]) > abs(tmp[0]) && abs(tmp[1]) > abs(tmp[2])) {
        axis = 1;
      }
      else if( abs(tmp[2]) > abs(tmp[1]) && abs(tmp[2]) > abs(tmp[0])){
        axis = 2;
      }

      std::vector<Obj> rightShapes;
      std::vector<Obj> leftShapes;
      for(int i=0; i < s.size(); i++) { 
        if ( mid[axis] >= s[i]->getBoundingBox().getMid()[axis] )
          rightShapes.push_back(s[i]);
        else
          leftShapes.push_back(s[i]);
      }

      if(leftShapes.size() == 0 && rightShapes.size() > 0) 
        leftShapes = rightShapes;
      else if(rightShapes.size() == 0 && leftShapes.size() > 0)
        rightShapes = leftShapes;

      double matches = 0;
      for(int i=0; i < leftShapes.size(); i++) {
        for(int j=0; j < rightShapes.size(); j++) {
          if ( leftShapes[i] == rightShapes[j] ) matches+=1.0;
        }
      }

      if ( matches/leftShapes.size() < 0.5 && matches/rightShapes.size() < 0.5 ){
        node->left = buildTree(leftShapes, depth - 1);
        node->right = buildTree(rightShapes, depth - 1);
      }
      else {
        node->left = new Node();
        node->left->shapes = std::vector<Obj>();
        node->right = new Node();
        node->right->shapes = std::vector<Obj>();
      }

      return node;
    }

    bool intersect(ray& r, isect& i) {
      Node<Obj>* node = this;
      double tMin, tMax;
      if (node->bb.intersect(r, tMin, tMax)){
        if( node->left->shapes.size() > 0 ){
          return node->left->intersect(r, i);
        }
        else if ( node->right->shapes.size() > 0 ){
          return node->right->intersect(r, i); 
        }
        else { // leaf
          for( int x = 0; x < node->shapes.size(); x++) {
            if (node->shapes[x]->intersect(r, i)) 
              return true;
          }
        }
      }
      return false;
    }

};


template <typename Obj>
class KdTree{
public:
  Node<Obj>* rootNode;

  KdTree(std::vector<Obj>& objects, int depth) {
    rootNode = Node<Obj>::buildTree(objects, depth);
  }

  bool intersect(ray& r, isect& i) {
    return rootNode->intersect(r, i);
  }
};

class SceneElement {

public:
  virtual ~SceneElement() {}

  Scene *getScene() const { return scene; }

  // For debugging purposes, draws using OpenGL
  virtual void glDraw(int quality, bool actualMaterials, bool actualTextures) const  { }

 protected:
 SceneElement( Scene *s )
   : scene( s ) {}

  Scene *scene;
};

class TransformNode {

protected:

  // information about this node's transformation
  Mat4d    xform;
  Mat4d    inverse;
  Mat3d    normi;

  // information about parent & children
  TransformNode *parent;
  std::vector<TransformNode*> children;
    
 public:
  typedef std::vector<TransformNode*>::iterator          child_iter;
  typedef std::vector<TransformNode*>::const_iterator    child_citer;

  ~TransformNode() {
      for(child_iter c = children.begin(); c != children.end(); ++c ) delete (*c);
    }

  TransformNode *createChild(const Mat4d& xform) {
    TransformNode *child = new TransformNode(this, xform);
    children.push_back(child);
    return child;
  }
    
  // Coordinate-Space transformation
  Vec3d globalToLocalCoords(const Vec3d &v) { return inverse * v; }

  Vec3d localToGlobalCoords(const Vec3d &v) { return xform * v; }

  Vec4d localToGlobalCoords(const Vec4d &v) { return xform * v; }

  Vec3d localToGlobalCoordsNormal(const Vec3d &v) {
    Vec3d ret = normi * v;
    ret.normalize();
    return ret;
  }

  const Mat4d& transform() const		{ return xform; }

protected:
  // protected so that users can't directly construct one of these...
  // force them to use the createChild() method.  Note that they CAN
  // directly create a TransformRoot object.
 TransformNode(TransformNode *parent, const Mat4d& xform ) : children() {
      this->parent = parent;
      if (parent == NULL) this->xform = xform;
      else this->xform = parent->xform * xform;  
      inverse = this->xform.inverse();
      normi = this->xform.upper33().inverse().transpose();
    }
};

class TransformRoot : public TransformNode {
 public:
 TransformRoot() : TransformNode(NULL, Mat4d()) {}
};

// A Geometry object is anything that has extent in three dimensions.
// It may not be an actual visible scene object.  For example, hierarchical
// spatial subdivision could be expressed in terms of Geometry instances.
class Geometry : public SceneElement {

protected:
  // intersections performed in the object's local coordinate space
  // do not call directly - this should only be called by intersect()
  virtual bool intersectLocal(ray& r, isect& i ) const = 0;

public:
  // intersections performed in the global coordinate space.
  bool intersect(ray& r, isect& i) const;

  virtual bool hasBoundingBoxCapability() const;
  const BoundingBox& getBoundingBox() const { return bounds; }
  Vec3d getNormal() { return Vec3d(1.0, 0.0, 0.0); }

  virtual void ComputeBoundingBox() {
    // take the object's local bounding box, transform all 8 points on it,
    // and use those to find a new bounding box.

    BoundingBox localBounds = ComputeLocalBoundingBox();
        
    Vec3d min = localBounds.getMin();
    Vec3d max = localBounds.getMax();

    Vec4d v, newMax, newMin;

    v = transform->localToGlobalCoords( Vec4d(min[0], min[1], min[2], 1) );
    newMax = v;
    newMin = v;
    v = transform->localToGlobalCoords( Vec4d(max[0], min[1], min[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(min[0], max[1], min[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(max[0], max[1], min[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(min[0], min[1], max[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(max[0], min[1], max[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(min[0], max[1], max[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
    v = transform->localToGlobalCoords( Vec4d(max[0], max[1], max[2], 1) );
    newMax = maximum(newMax, v);
    newMin = minimum(newMin, v);
		
    bounds.setMax(Vec3d(newMax));
    bounds.setMin(Vec3d(newMin));
  }

  // default method for ComputeLocalBoundingBox returns a bogus bounding box;
  // this should be overridden if hasBoundingBoxCapability() is true.
  virtual BoundingBox ComputeLocalBoundingBox() { return BoundingBox(); }

  void setTransform(TransformNode *transform) { this->transform = transform; };
    
 Geometry(Scene *scene) : SceneElement( scene ) {}

  // For debugging purposes, draws using OpenGL
  void glDraw(int quality, bool actualMaterials, bool actualTextures) const;

  // The defult does nothing; this is here because it is not required
  // that you implement this function if you create your own scene objects.
  virtual void glDrawLocal(int quality, bool actualMaterials, bool actualTextures) const { }

 protected:
  BoundingBox bounds;
  TransformNode *transform;
};

// A SceneObject is a real actual thing that we want to model in the 
// world.  It has extent (its Geometry heritage) and surface properties
// (its material binding).  The decision of how to store that material
// is left up to the subclass.
class SceneObject : public Geometry {

 public:
  virtual const Material& getMaterial() const = 0;
  virtual void setMaterial(Material *m) = 0;

  void glDraw(int quality, bool actualMaterials, bool actualTextures) const;

 protected:
 SceneObject( Scene *scene )
   : Geometry( scene ) {}
};

// A simple extension of SceneObject that adds an instance of Material
// for simple material bindings.
class MaterialSceneObject : public SceneObject {

public:
  virtual ~MaterialSceneObject() { delete material; }

  virtual const Material& getMaterial() const { return *material; }
  virtual void setMaterial(Material* m)	{ delete material; material = m; }

protected:
 MaterialSceneObject(Scene *scene, Material *mat) 
   : SceneObject(scene), material(mat) {}

  Material* material;
};

class Scene {

public:
  typedef std::vector<Light*>::iterator	liter;
  typedef std::vector<Light*>::const_iterator cliter;
  typedef std::vector<Geometry*>::iterator giter;
  typedef std::vector<Geometry*>::const_iterator cgiter;

  TransformRoot transformRoot;

  Scene() : transformRoot(), objects(), lights() {}
  virtual ~Scene();

  void add( Geometry* obj ) {
    obj->ComputeBoundingBox();
	sceneBounds.merge(obj->getBoundingBox());
    objects.push_back(obj);
  }
  void add(Light* light) { lights.push_back(light); }

  bool intersect(ray& r, isect& i) const;

  std::vector<Light*>::const_iterator beginLights() const { return lights.begin(); }
  std::vector<Light*>::const_iterator endLights() const { return lights.end(); }

  std::vector<Geometry*>::const_iterator beginObjects() const { return objects.begin(); }
  std::vector<Geometry*>::const_iterator endObjects() const { return objects.end(); }
        
  const Camera& getCamera() const { return camera; }
  Camera& getCamera() { return camera; }

  // For efficiency reasons, we'll store texture maps in a cache
  // in the Scene.  This makes sure they get deleted when the scene
  // is destroyed.
  TextureMap* getTexture( string name );

  // These two functions are for handling ambient light; in the Phong model,
  // the "ambient" light is considered a property of the _scene_ as a whole
  // and hence should be set here.
  Vec3d ambient() const	{ return ambientIntensity; }
  void addAmbient( const Vec3d& ambient ) { ambientIntensity += ambient; }

  void glDraw(int quality, bool actualMaterials, bool actualTextures) const;

  const BoundingBox& bounds() const { return sceneBounds; }

  void buildKdTree(int depth) {
    kdtree = new KdTree<Geometry*>(objects, depth);
    std::cout << "KdTree built" << std::endl;
  }

  bool kdIntersect(ray& r, isect& i){
    return kdtree->intersect(r, i);
  }

 private:
  std::vector<Geometry*> objects;
  std::vector<Geometry*> nonboundedobjects;
  std::vector<Geometry*> boundedobjects;
  std::vector<Light*> lights;
  Camera camera;

  // This is the total amount of ambient light in the scene
  // (used as the I_a in the Phong shading model)
  Vec3d ambientIntensity;

  typedef std::map< std::string, TextureMap* > tmap;
  tmap textureCache;
	
  // Each object in the scene, provided that it has hasBoundingBoxCapability(),
  // must fall within this bounding box.  Objects that don't have hasBoundingBoxCapability()
  // are exempt from this requirement.
  BoundingBox sceneBounds;
  
  KdTree<Geometry*>* kdtree;

 public:
  // This is used for debugging purposes only.
  mutable std::vector<std::pair<ray*, isect*> > intersectCache;
};

#endif // __SCENE_H__
