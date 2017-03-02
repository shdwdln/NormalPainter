#include "pch.h"
#include "MeshSync/msServer.h"
#include "MeshSyncServer.h"

using namespace mu;

static std::map<uint16_t, ms::Server*> g_servers;

msAPI ms::Server* msServerStart(const ms::ServerSettings *settings)
{
    if (!settings) { return nullptr; }

    ms::Server *ret = g_servers[settings->port];
    if (!ret) {
        ret = new ms::Server(*settings);
        ret->start();
        g_servers[settings->port] = ret;
    }
    else {
        ret->setServe(true);
        ret->getSettings() = *settings;
    }
    return ret;
}

msAPI void  msServerStop(ms::Server *server)
{
    // actually not stop. just make server ignore further requests.
    if (server) {
        server->setServe(false);
    }
}

msAPI int msServerProcessMessages(ms::Server *server, msMessageHandler handler)
{
    if (!server || !handler) { return 0; }
    return server->processMessages([handler](ms::MessageType type, const ms::Message& data) {
        handler(type, &data);
    });
}

msAPI void msServerBeginServe(ms::Server *server)
{
    if (!server) { return; }
    server->beginServe();
}
msAPI void msServerEndServe(ms::Server *server)
{
    if (!server) { return; }
    server->endServe();
}
msAPI void msServerServeMesh(ms::Server *server, ms::Mesh *data)
{
    if (!server) { return; }
    server->getHostScene()->meshes.emplace_back(data);
}
msAPI void msServerSetNumMaterials(ms::Server *server, int n)
{
    if (!server) { return; }
    server->getHostScene()->materials.resize(n);
}


msAPI void msServerSetScreenshotFilePath(ms::Server *server, const char *path)
{
    if (!server) { return; }
    server->setScrrenshotFilePath(path);
}

msAPI int msGetGetBakeSkin(ms::GetMessage *_this)
{
    return _this->refine_settings.flags.bake_skin;
}
msAPI int msGetGetBakeCloth(ms::GetMessage *_this)
{
    return _this->refine_settings.flags.bake_cloth;
}

msAPI ms::Scene* msSetGetSceneData(ms::SetMessage *_this)
{
    return &_this->scene;
}

msAPI int msMaterialGetID(ms::Material *_this)
{
    return _this->id;
}
msAPI void msMaterialSetID(ms::Material *_this, int v)
{
    _this->id = v;
}
msAPI const char* msMaterialGetName(ms::Material *_this)
{
    return _this->name.c_str();
}
msAPI void msMaterialSetName(ms::Material *_this, const char *v)
{
    _this->name = v;
}
msAPI float4 msMaterialGetColor(ms::Material *_this)
{
    return _this->color;
}
msAPI void msMaterialSetColor(ms::Material *_this, const float4 *v)
{
    _this->color = *v;
}

msAPI float   msKeyframeGetTime(ms::Keyframe *_this) { return _this->time; }
//msAPI float2  msKeyframeGetInTangent(ms::Keyframe *_this) { return _this->in_tangent; }
//msAPI float2  msKeyframeGetOutTangent(ms::Keyframe *_this) { return _this->out_tangent; }
msAPI float   msKeyframeGetValue(ms::Keyframe *_this) { return _this->value; }

msAPI int           msAnimationGetNumTranslationXKeys(ms::Animation *_this)     { return _this ? (int)_this->translation.x.size() : 0; }
msAPI int           msAnimationGetNumTranslationYKeys(ms::Animation *_this)     { return _this ? (int)_this->translation.y.size() : 0; }
msAPI int           msAnimationGetNumTranslationZKeys(ms::Animation *_this)     { return _this ? (int)_this->translation.z.size() : 0; }
msAPI ms::Keyframe* msAnimationGetTranslationXKey(ms::Animation *_this, int i)  { return &_this->translation.x[i]; }
msAPI ms::Keyframe* msAnimationGetTranslationYKey(ms::Animation *_this, int i)  { return &_this->translation.y[i]; }
msAPI ms::Keyframe* msAnimationGetTranslationZKey(ms::Animation *_this, int i)  { return &_this->translation.z[i]; }

msAPI int           msAnimationGetNumRotationXKeys(ms::Animation *_this)        { return _this ? (int)_this->rotation.x.size() : 0; }
msAPI int           msAnimationGetNumRotationYKeys(ms::Animation *_this)        { return _this ? (int)_this->rotation.y.size() : 0; }
msAPI int           msAnimationGetNumRotationZKeys(ms::Animation *_this)        { return _this ? (int)_this->rotation.z.size() : 0; }
msAPI ms::Keyframe* msAnimationGetRotationXKey(ms::Animation *_this, int i)     { return &_this->rotation.x[i]; }
msAPI ms::Keyframe* msAnimationGetRotationYKey(ms::Animation *_this, int i)     { return &_this->rotation.y[i]; }
msAPI ms::Keyframe* msAnimationGetRotationZKey(ms::Animation *_this, int i)     { return &_this->rotation.z[i]; }

msAPI int           msAnimationGetNumScaleXKeys(ms::Animation *_this)           { return _this ? (int)_this->scale.x.size() : 0; }
msAPI int           msAnimationGetNumScaleYKeys(ms::Animation *_this)           { return _this ? (int)_this->scale.y.size() : 0; }
msAPI int           msAnimationGetNumScaleZKeys(ms::Animation *_this)           { return _this ? (int)_this->scale.z.size() : 0; }
msAPI ms::Keyframe* msAnimationGetScaleXKey(ms::Animation *_this, int i)        { return &_this->scale.x[i]; }
msAPI ms::Keyframe* msAnimationGetScaleYKey(ms::Animation *_this, int i)        { return &_this->scale.y[i]; }
msAPI ms::Keyframe* msAnimationGetScaleZKey(ms::Animation *_this, int i)        { return &_this->scale.z[i]; }

msAPI int           msAnimationGetNumVisibilityKeys(ms::Animation *_this)       { return _this ? (int)_this->visibility.size() : 0; }
msAPI ms::Keyframe* msAnimationGetVisibilityKey(ms::Animation *_this, int i)    { return &_this->visibility[i]; }

msAPI ms::GetFlags msGetGetFlags(ms::GetMessage *_this)
{
    return _this->flags;
}

msAPI int msDeleteGetNumTargets(ms::DeleteMessage *_this)
{
    return (int)_this->targets.size();
}
msAPI const char* msDeleteGetPath(ms::DeleteMessage *_this, int i)
{
    return _this->targets[i].path.c_str();
}
msAPI int msDeleteGetID(ms::DeleteMessage *_this, int i)
{
    return _this->targets[i].id;
}

msAPI ms::FenceMessage::FenceType msFenceGetType(ms::FenceMessage *_this)
{
    return _this->type;
}

msAPI const char* msTextGetText(ms::TextMessage *_this)
{
    return _this->text.c_str();
}
msAPI ms::TextMessage::Type msTextGetType(ms::TextMessage *_this)
{
    return _this->type;
}


msAPI ms::Transform* msTransformCreate()
{
    return new ms::Transform();
}
msAPI int msTransformGetID(ms::Transform *_this)
{
    return _this->id;
}
msAPI void msTransformSetID(ms::Transform *_this, int v)
{
    _this->id = v;
}
msAPI int msTransformGetIndex(ms::Transform *_this)
{
    return _this->index;
}
msAPI void msTransformSetIndex(ms::Transform *_this, int v)
{
    _this->index = v;
}
msAPI const char* msTransformGetPath(ms::Transform *_this)
{
    return _this->path.c_str();
}
msAPI void msTransformSetPath(ms::Transform *_this, const char *v)
{
    _this->path = v;
}
msAPI void msTransformGetTRS(ms::Transform *_this, ms::TRS *dst)
{
    *dst = _this->transform;
}
msAPI void msTransformSetTRS(ms::Transform *_this, const ms::TRS *v)
{
    _this->transform = *v;
}
msAPI const char* msTransformGetReference(ms::Transform *_this)
{
    return _this->reference.c_str();
}
msAPI void msTransformSetReference(ms::Transform *_this, const char *v)
{
    _this->reference = *v;
}
msAPI ms::Animation* msTransformGetAnimation(ms::Transform *_this)
{
    return _this->animation.get();
}

msAPI ms::Camera* msCameraCreate()
{
    return new ms::Camera();
}
msAPI float msCameraGetFov(ms::Camera *_this)
{
    return _this->fov;
}
msAPI void msCameraSetFov(ms::Camera *_this, float v)
{
    _this->fov = v;
}


msAPI ms::Mesh* msMeshCreate()
{
    return new ms::Mesh();
}
msAPI ms::MeshDataFlags msMeshGetFlags(ms::Mesh *_this)
{
    return _this->flags;
}
msAPI void msMeshSetFlags(ms::Mesh *_this, ms::MeshDataFlags v)
{
    _this->flags = v;
}
msAPI int msMeshGetNumPoints(ms::Mesh *_this)
{
    return (int)_this->points.size();
}
msAPI int msMeshGetNumIndices(ms::Mesh *_this)
{
    return (int)_this->indices.size();
}
msAPI int msMeshGetNumSplits(ms::Mesh *_this)
{
    return (int)_this->splits.size();
}
msAPI void msMeshReadPoints(ms::Mesh *_this, float3 *dst)
{
    _this->points.copy_to(dst);
}
msAPI void msMeshWritePoints(ms::Mesh *_this, const float3 *v, int size)
{
    _this->points.assign(v, v + size);
    _this->flags.has_points = 1;
}
msAPI void msMeshReadNormals(ms::Mesh *_this, float3 *dst)
{
    _this->normals.copy_to(dst);
}
msAPI void msMeshWriteNormals(ms::Mesh *_this, const float3 *v, int size)
{
    _this->normals.assign(v, v + size);
    _this->flags.has_normals = 1;
}
msAPI void msMeshReadTangents(ms::Mesh *_this, float4 *dst)
{
    _this->tangents.copy_to(dst);
}
msAPI void msMeshWriteTangents(ms::Mesh *_this, const float4 *v, int size)
{
    _this->tangents.assign(v, v + size);
    _this->flags.has_tangents = 1;
}
msAPI void msMeshReadUV(ms::Mesh *_this, float2 *dst)
{
    _this->uv.copy_to(dst);
}
msAPI void msMeshWriteUV(ms::Mesh *_this, const float2 *v, int size)
{
    _this->uv.assign(v, v + size);
    _this->flags.has_uv = 1;
}
msAPI void msMeshReadIndices(ms::Mesh *_this, int *dst)
{
    _this->indices.copy_to(dst);
}
msAPI void msMeshWriteIndices(ms::Mesh *_this, const int *v, int size)
{
    _this->indices.assign(v, v + size);
    _this->flags.has_indices = 1;
    _this->flags.visible = 1;
}
msAPI void msMeshWriteSubmeshTriangles(ms::Mesh *_this, const int *v, int size, int materialID)
{
    {
        _this->indices.insert(_this->indices.end(), v, v + size);
    }
    {
        size_t pos = _this->materialIDs.size();
        _this->materialIDs.resize(pos + size / 3);
        std::fill_n(_this->materialIDs.data() + pos, size / 3, materialID);
    }
    _this->flags.has_indices = 1;
    _this->flags.has_materialIDs = 1;
    _this->flags.visible = 1;
}
msAPI ms::SplitData* msMeshGetSplit(ms::Mesh *_this, int i)
{
    return &_this->splits[i];
}

msAPI int msMeshGetNumSubmeshes(ms::Mesh *_this)
{
    return (int)_this->submeshes.size();
}
msAPI ms::SubmeshData* msMeshGetSubmesh(ms::Mesh *_this, int i)
{
    return &_this->submeshes[i];
}

msAPI int msMeshGetNumWeights4(ms::Mesh *_this)
{
    return (int)_this->weights4.size();
}
msAPI void msMeshReadWeights4(ms::Mesh *_this, ms::Weights4 *v)
{
    _this->weights4.copy_to(v);
}
msAPI void msMeshWriteWeights4(ms::Mesh *_this, const ms::Weights4 *v, int size)
{
    _this->weights4.assign(v, v + size);
}
msAPI int msMeshGetNumBones(ms::Mesh *_this)
{
    return (int)_this->bones.size();
}
msAPI const char* msMeshGetBonePath(ms::Mesh *_this, int i)
{
    return _this->bones[i].c_str();
}
msAPI void msMeshSetBonePath(ms::Mesh *_this, const char *v, int i)
{
    if (i + 1 >= _this->bones.size()) {
        _this->bones.resize(i + 1);
    }
    _this->bones[i] = v;
}
msAPI void msMeshReadBindPoses(ms::Mesh *_this, float4x4 *v)
{
    _this->bindposes.copy_to(v);
}
msAPI void msMeshWriteBindPoses(ms::Mesh *_this, const float4x4 *v, int size)
{
    _this->bindposes.assign(v, v + size);
}

msAPI void msMeshSetLocal2World(ms::Mesh *_this, const float4x4 *v)
{
    _this->refine_settings.local2world = *v;
}
msAPI void msMeshSetWorld2Local(ms::Mesh *_this, const float4x4 *v)
{
    _this->refine_settings.world2local = *v;
}


msAPI int msSplitGetNumPoints(ms::SplitData *_this)
{
    return (int)_this->points.size();
}
msAPI int msSplitGetNumIndices(ms::SplitData *_this)
{
    return (int)_this->indices.size();
}
msAPI int msSplitGetNumSubmeshes(ms::SplitData *_this)
{
    return (int)_this->submeshes.size();
}
msAPI void msSplitReadPoints(ms::SplitData *_this, float3 *dst)
{
    _this->points.copy_to(dst);
}
msAPI void msSplitReadNormals(ms::SplitData *_this, float3 *dst)
{
    _this->normals.copy_to(dst);
}
msAPI void msSplitReadTangents(ms::SplitData *_this, float4 *dst)
{
    _this->tangents.copy_to(dst);
}
msAPI void msSplitReadUV(ms::SplitData *_this, float2 *dst)
{
    _this->uv.copy_to(dst);
}
msAPI void msSplitReadWeights4(ms::SplitData *_this, ms::Weights4 *dst)
{
    _this->weights4.copy_to(dst);
}
msAPI void msSplitReadIndices(ms::SplitData *_this, int *dst)
{
    _this->indices.copy_to(dst);
}
msAPI ms::SubmeshData* msSplitGetSubmesh(ms::SplitData *_this, int i)
{
    return &_this->submeshes[i];
}

msAPI int msSubmeshGetNumIndices(ms::SubmeshData *_this)
{
    return (int)_this->indices.size();
}
msAPI int msSubmeshGetMaterialID(ms::SubmeshData *_this)
{
    return _this->materialID;
}
msAPI void msSubmeshReadIndices(ms::SubmeshData *_this, int *dst)
{
    _this->indices.copy_to(dst);
}


msAPI int msSceneGetNumMeshes(ms::Scene *_this)
{
    return (int)_this->meshes.size();
}
msAPI ms::Mesh* msSceneGetMeshData(ms::Scene *_this, int i)
{
    return _this->meshes[i].get();
}
msAPI int msSceneGetNumTransforms(ms::Scene *_this)
{
    return (int)_this->transforms.size();
}
msAPI ms::Transform* msSceneGetTransformData(ms::Scene *_this, int i)
{
    return _this->transforms[i].get();
}
msAPI int msSceneGetNumCameras(ms::Scene *_this)
{
    return (int)_this->cameras.size();
}
msAPI ms::Camera* msSceneGetCameraData(ms::Scene *_this, int i)
{
    return _this->cameras[i].get();
}
msAPI int msSceneGetNumMaterials(ms::Scene *_this)
{
    return (int)_this->materials.size();
}
msAPI ms::Material* msSceneGetMaterialData(ms::Scene *_this, int i)
{
    return &_this->materials[i];
}
